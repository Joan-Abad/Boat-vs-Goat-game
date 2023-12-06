#include <iostream>
#include <SFML/Network.hpp>
#include "NetworkingManagerServer.h"
#include <json.h>
/*
NetworkingManagerServer::NetworkingManagerServer() 
{
	std::cout << "PLAYING ON SERVER\n";
}

void NetworkingManagerServer::Bind(int port)
{
	this->port = port;
	socket->bind(port);
	socket->setBlocking(false);
}

void NetworkingManagerServer::SendPackages()
{
	for (auto& packetToSend : packetsToSend)
	{

		sf::Socket::Status status = socket->send(packetToSend, "192.168.68.110", port);
		std::cout << "Sending packet";

	}
	packetsToSend.clear();
}

void NetworkingManagerServer::ListenIncomingPackages()
{
	sf::Packet packet;
	sf::IpAddress clientAddress;
	unsigned short clientPort;

	if (socket->receive(packet, clientAddress, clientPort) == sf::Socket::Done && clientAddress != "127.0.0.1")
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
*/