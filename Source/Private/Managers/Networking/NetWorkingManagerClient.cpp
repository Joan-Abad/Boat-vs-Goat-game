#include "Managers/Networking/NetworkingManagerClient.h"
#include <iostream>
#include "GameObjects/Players/Boat.h"
#include "Managers/GameManager.h"
#include "Map/Map.h"

unsigned short NetworkingManagerClient::processPort = 53001;
unsigned short NetworkingManagerClient::serverPort = 53001;

NetworkingManagerClient::NetworkingManagerClient(const sf::IpAddress& ipAddress, const unsigned int port) : clientManagementData(EClientManagementData::EConnectingToServer)
{
	serverAddress = ipAddress;
	this->processPort = port;
}

void NetworkingManagerClient::OnInit()
{
	udpSocket.bind(processPort);

	udpSocket.setBlocking(false);

	SetInitialPacketToServer();
}

void NetworkingManagerClient::UpdateNetworkData()
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
		RecieveDataFromServer();
		SendGameDataToServer();
		break;
	case EClientManagementData::EEndMatch:
		break;
	default:
		break;
	}
}

void NetworkingManagerClient::WaitForServerResponse()
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

void NetworkingManagerClient::WaitForGameStart()
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
			unsigned short matchNumPlayers = root[numPlayers].asInt();
			playerID = root[key_PlayerID].asInt();

			if (bStartMatch)
			{
				clientManagementData = EClientManagementData::EPlayMatch;
				GameManager* gm = GameManager::GetGameManager();
				gm->InitGameWindow();
				gm->InitGameMap(gm->GetMap(gm->LakeMap), matchNumPlayers);
			}
			else
				std::cout << "Data not correct\n";
		}
	}
}

void NetworkingManagerClient::RecieveDataFromServer()
{
	sf::Packet packet;
	sf::IpAddress ipAddress; 
	unsigned short port; 
	//recieve packet from clients and check if they want to access the game 
	if (udpSocket.receive(packet, ipAddress, port) == sf::Socket::Done)
	{
		std::string message;
		packet >> message;

		std::cout << "Client: Recieved a message from " << serverAddress << " on port " << gamePort << ": \n";
		std::cout << message;

		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(message, root);
		if (parsingSuccessful)
		{
			Map& map = *GameManager::GetGameManager()->GetCurrentMap();
			Json::StreamWriterBuilder writerBuilder;
			std::string rootmsg = Json::writeString(writerBuilder, root);
			//Map data should not be an array. Need to check what to do
   			if (root.isMember("mapData") && root["mapData"].isArray())
			{
				const Json::Value& mapsData = root["mapData"];

				for (const auto& mapData : mapsData)
				{
					std::string mapDataString = Json::writeString(writerBuilder, mapData);

					map.UpdateClientNetData(mapData);
				}
			}

			if (root.isMember("gameObjects") && root["gameObjects"].isArray())
			{

  				const Json::Value& gameObjects = root["gameObjects"];
 				std::string gameObjt = Json::writeString(writerBuilder, gameObjects);
				for (const auto& gameObject : gameObjects)
				{
					std::string goData = Json::writeString(writerBuilder, gameObject);

					if (gameObject.isMember(GameObject::key_gameObjectID))
					{
						int goID = gameObject[GameObject::key_gameObjectID].asInt();
						
						map.GetGameObjects()[goID]->UpdateClientNetData(gameObject);
					}
				}
			}
			
		}

	}
}

void NetworkingManagerClient::SendGameDataToServer()
{
	//If root data has some content to send, proceed. 
	bool hasGameObjectsData = !GetGameObjectsNetData().empty();
	bool hasMapObjectsData = !GetMapNetData().empty();

	if (hasGameObjectsData || hasMapObjectsData)
	{
		sf::Packet packet;

		Json::StreamWriterBuilder writerBuilder;


		if (hasMapObjectsData)
		{
			std::string mapMsg;
			GetMapNetData()["mapData"] = GetMapNetData();
			mapMsg = Json::writeString(writerBuilder, GetMapNetData());
			packet << mapMsg;

		}
		if (hasGameObjectsData)
		{
			std::string goData; 
			GetRootData()["gameObjects"] = GetGameObjectsNetData();
			goData = Json::writeString(writerBuilder, GetRootData());
			packet << goData;
		}

		udpSocket.send(packet, serverAddress, gamePort);

		ClearNetData();
	}
}


void NetworkingManagerClient::SetInitialPacketToServer()
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
