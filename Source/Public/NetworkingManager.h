#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <json.h>
#include "GameManager.h"

#define LAN 1

enum class EServerManagementData
{
	EWaitingPlayers,
	EStartingTheMatch,
	EStartTheMatch,
	EPlayMatch,
	EEndMatch
};

enum class EClientManagementData
{
	EConnectingToServer, 
	EWaitingForPlayers, 
	EStartingTheMatch,
	EPlayMatch,
	EEndMatch
};


struct PlayerConnectionInfo
{
	PlayerConnectionInfo() = default;

	PlayerConnectionInfo(sf::IpAddress ip, int port)
	{
		ipAddress = ip; 
		this->port = port;
	}

	sf::IpAddress ipAddress;
	int port;
};

class NetworkingManager
{
public:
	NetworkingManager()
	{
		//OnInit();
	}

	virtual ~NetworkingManager() = default;
	
	virtual void OnInit()
	{
	};

	//This function should be the one that handles how the data is send.
	virtual void UpdateNetworkData() = 0;

	NetworkingManager(const NetworkingManager&) = delete;
	NetworkingManager& operator=(const NetworkingManager&) = delete;
	const static unsigned short GetGamePort() { return gamePort; };
protected:
	sf::UdpSocket udpSocket; 
	std::vector<sf::Packet> packetsToSendThisFrame;
	
	//Communication keys
	const static std::string accessKey;
	const static std::string connectionWithServer; 
	const static std::string startGameKey;

	static unsigned short gamePort;
}; 

class NetworkingManagerServer : public NetworkingManager
{
public: 
	NetworkingManagerServer() : NetworkingManager(), serverManagementData(EServerManagementData::EWaitingPlayers), numPlayersToStartTheGame(2)
	{

	}

	virtual void OnInit() override
	{
		udpSocket.bind(gamePort);

		//Set the server as its own player
		players["player0"] = PlayerConnectionInfo(sf::IpAddress::getLocalAddress(), gamePort);

		udpSocket.setBlocking(false);
	}

	virtual void UpdateNetworkData() override
	{
		switch (serverManagementData)
		{
		case EServerManagementData::EWaitingPlayers:
			WaitForClientsToConnect();
			break;
		case EServerManagementData::EStartingTheMatch:
			std::cout << "Server: Starting the match\n";
			StartGameServerAndClients();
			break;
			//This case is if we want to add a countdown to start the match
		case EServerManagementData::EStartTheMatch:
			break;
		case EServerManagementData::EPlayMatch:
			std::cout << "Server On Match\n";
			break;
		case EServerManagementData::EEndMatch:
			break;
		default:
			break;
		}
	}
private: 
	//Code that waits for clients to connect and pass througt the Strarting Match state
	void WaitForClientsToConnect()
	{
		if (bDisplayMessageWaitingForClients)
		{
			std::cout << "Waiting for clients to connect...\n";
			bDisplayMessageWaitingForClients = false; 
		}

		//Wait until the game has reached the number of players able to start the game
		if (players.size() != numPlayersToStartTheGame)
		{
			sf::Packet packet;
			sf::IpAddress clientAddress;
			unsigned short clientPort;

			//recieve packet from clients and check if they want to access the game 
			if (udpSocket.receive(packet, clientAddress, clientPort) == sf::Socket::Done)
			{
				std::string message;
				packet >> message;

				std::cout << "Server: Recieved a message from " << clientAddress << " on port " << clientPort << ": \n";
				std::cout << message;

				Json::Value root;
				Json::Reader reader;
				bool parsingSuccessful = reader.parse(message, root);
				if (parsingSuccessful)
				{
					//Add the player if has the right variables
					bool bWantToPlay = root[accessKey].asBool();
					if (bWantToPlay)
					{
						std::cout << "\nServer: Player " << players.size() << " connected\n";
						players["Player" + players.size()] = PlayerConnectionInfo(clientAddress, clientPort);

						//Send a response to the player
						sf::Packet ConfirmationToThePlayer; 

						Json::Value replyRoot;
						replyRoot[connectionWithServer] = true;

						Json::StreamWriterBuilder writerBuilder;
						std::string acceptedByServer = Json::writeString(writerBuilder, replyRoot);
						ConfirmationToThePlayer << acceptedByServer;


						sf::Socket::Status status = udpSocket.send(ConfirmationToThePlayer, clientAddress, clientPort);
					}
					else
					{
						std::cerr << "Recieved some unnexpected data: " << message << std::endl;
					}

				}
			}
		}
		else
		{
			serverManagementData = EServerManagementData::EStartingTheMatch;
		}
	}

	void StartGameServerAndClients()
	{
		for (auto& player : players)
		{
			//Send the start the game flag to clients
			sf::Packet packet_StartTheGame;
			Json::Value root;
			root[startGameKey] = true;

			Json::StreamWriterBuilder writerBuilder;
			std::string msgToSend = Json::writeString(writerBuilder, root);
			packet_StartTheGame << msgToSend;

			sf::Socket::Status status = udpSocket.send(packet_StartTheGame, player.second.ipAddress, player.second.port);
		}
		serverManagementData = EServerManagementData::EPlayMatch;
		GameManager::GetGameManager()->SetStartGame(true);
	}

	//This array holds the info to recieve and send information of the players
	std::unordered_map<std::string, PlayerConnectionInfo> players;

	//Variable that manages the network data of the game from the server side
	EServerManagementData serverManagementData;

	//Number of players needed to start the game
	int numPlayersToStartTheGame; 

	bool bDisplayMessageWaitingForClients = true; 
};

class NetworkingManagerClient : public NetworkingManager
{
public: 
	NetworkingManagerClient(const sf::IpAddress& ipAddress, const unsigned int port) : NetworkingManager(), clientManagementData(EClientManagementData::EConnectingToServer)
	{
		serverAddress = ipAddress;
		this->processPort = port; 
	}

	virtual void OnInit() override
	{
		udpSocket.bind(processPort);

		udpSocket.setBlocking(false);
		
		SetInitialPacketToServer();		
	}

	virtual void UpdateNetworkData() override
	{
		switch (clientManagementData)
		{
		case EClientManagementData::EConnectingToServer:
			WaitForServerResponse();
			break;
		case EClientManagementData::EWaitingForPlayers:
			if (bDisplayMessageWaitingOtherPlayers)
			{
				std::cout << "\n\nConnected to the server!";
				std::cout << "\nWaiting for players to connect...\n";
				bDisplayMessageWaitingOtherPlayers = false; 
			}
			WaitForGameStart();
			break;
		case EClientManagementData::EStartingTheMatch:
			break;
		case EClientManagementData::EPlayMatch:
			std::cout << "Client on match! \n";
			break;
		case EClientManagementData::EEndMatch:
			break;
		default:
			break;
		}
	}
private: 
	void WaitForServerResponse()
	{
		//Wait the response of the server
		sf::Packet packet;
		sf::IpAddress clientAddress;
		unsigned short clientPort;

		//recieve packet from clients and check if they want to access the game 
		if (udpSocket.receive(packet, clientAddress, clientPort) == sf::Socket::Done)
		{
			std::string message;
			packet >> message;

			std::cout << "Client: Recieved a message from " << clientAddress << " on port " << clientPort << ": \n";
			std::cout << "Content: \n";
			std::cout << message;

			Json::Value root;
			Json::Reader reader;
			bool parsingSuccessful = reader.parse(message, root);
			if (parsingSuccessful)
			{
				bool bCanPlay = root[connectionWithServer].asBool();
				if (bCanPlay)
					clientManagementData = EClientManagementData::EWaitingForPlayers;
				else
					std::cout << "Data not correct\n";
			}
		}
	}

	void WaitForGameStart()
	{
		sf::Packet packet;
		sf::IpAddress clientAddress;
		unsigned short clientPort;

		//recieve packet from clients and check if they want to access the game 
		if (udpSocket.receive(packet, clientAddress, clientPort) == sf::Socket::Done)
		{
			std::string message;
			packet >> message;

			std::cout << "Client: Recieved a message from " << clientAddress << " on port " << clientPort << ": \n";
			std::cout << "Content: \n";
			std::cout << message;

			Json::Value root;
			Json::Reader reader;
			bool parsingSuccessful = reader.parse(message, root);
			if (parsingSuccessful)
			{
				bool bStartMatch = root[startGameKey].asBool();
				if (bStartMatch)
				{
					clientManagementData = EClientManagementData::EPlayMatch;
					GameManager::GetGameManager()->SetStartGame(true);
				}
				else
					std::cout << "Data not correct\n";
			}
		}
	}

	void SetInitialPacketToServer()
	{
		//Send the access key to the server
		sf::Packet packet;
		Json::Value root;
		root[accessKey] = true;

		Json::StreamWriterBuilder writerBuilder;
		std::string msgToSend = Json::writeString(writerBuilder, root);
		packet << msgToSend;

		sf::Socket::Status status = udpSocket.send(packet, serverAddress, gamePort);
	}
	sf::IpAddress serverAddress; 

	bool bDisplayMessageWaitingOtherPlayers = true; 
	static unsigned short processPort; 

	//Manages the network data of the game from the client side
	EClientManagementData clientManagementData; 
};

//public:
//    template<typename NetworkManager>
//    static NetworkingManager& GetNetworkingManager()
//    {
//        std::call_once(onceFlag, []() {
//            instance = new NetworkManager();
//            });
//
//        return *instance;
//    }
//    using OnNetworkDataRecieved = std::function<void(const std::string&)>;
//
//    std::vector<OnNetworkDataRecieved> NetworkDataRecievedCallbacks;
//
//    template<typename FncAdd, typename FncObject>
//    void AddNetworkDataCallback(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
//    {
//        NetworkDataRecievedCallbacks.push_back(std::bind(FunctionAddress, ObjectOwningFunction, std::placeholders::_1));
//    }
//
//    
//
//    virtual void SendPackages();
//    virtual void Bind(int port) = 0;
//    virtual void ListenIncomingPackages();
//    void AddPacketToSend(sf::Packet packet);
//
//
//protected:
//    NetworkingManager();
//    ~NetworkingManager();
//    sf::UdpSocket* socket;
//    std::vector<sf::Packet> packetsToSend;
//    int port; 
//private:
//    static std::once_flag onceFlag;
//    static NetworkingManager* instance;
//
//    NetworkingManager(const NetworkingManager&) = delete;
//    NetworkingManager& operator=(const NetworkingManager&) = delete;
