#include "Managers/Networking/NetworkingManagerClient.h"
#include <iostream>

unsigned short NetworkingManagerClient::processPort = 53001;

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
		std::cout << "Client on match! \n";
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
			if (bStartMatch)
			{
				clientManagementData = EClientManagementData::EPlayMatch;
				GameManager::GetGameManager()->InitGameWindow();
			}
			else
				std::cout << "Data not correct\n";
		}
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
