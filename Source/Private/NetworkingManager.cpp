#include "NetworkingManager.h"
#include <iostream>

unsigned short NetworkingManager::gamePort = 53001;
const std::string NetworkingManager::accessKey = "bWantsToPlay";
const std::string NetworkingManager::connectionWithServer = "bAcceptedByServer";
const std::string NetworkingManager::startGameKey = "bStartGame";
unsigned short NetworkingManagerClient::processPort = 53001;

/*
//sf::UdpSocket* NetworkingManager::socket;
std::once_flag NetworkingManager::onceFlag;
NetworkingManager* NetworkingManager::instance = nullptr;

NetworkingManager::NetworkingManager() : port(53001)
{
	socket = new sf::UdpSocket();
}

NetworkingManager::~NetworkingManager()
{
	delete socket; 
	delete instance; 
}


void NetworkingManager::SendPackages()
{
	for (auto& packetToSend : packetsToSend)
	{
		
		sf::Socket::Status status = socket->send(packetToSend, "127.0.0.1", port);
		std::cout << "Sending packet";
			
	}
	packetsToSend.clear(); 
}

void NetworkingManager::ListenIncomingPackages()
{
	sf::Packet packet;
	sf::IpAddress clientAddress;
	unsigned short clientPort;

	if (socket->receive(packet, clientAddress, clientPort) == sf::Socket::Done)
	{
		std::string message;
		packet >> message;

		for (auto& recievedDataFunction : NetworkDataRecievedCallbacks)
		{
			recievedDataFunction(message);
		}

		std::cout << "Received message from " << clientAddress << ":" << clientPort << ": " << message << std::endl;
	}
}

void NetworkingManager::AddPacketToSend(sf::Packet packet)
{
	packetsToSend.push_back(packet);
}
*/