#include <iostream>
#include <SFML/Network.hpp>
#include "NetworkingManagerServer.h"

NetworkingManagerServer::NetworkingManagerServer() : NetworkingManager()
{
	std::cout << "PLAYING ON SERVER\n";
}

void NetworkingManagerServer::Bind(int port)
{
	this->port = port;
	socket->bind(port);
	socket->setBlocking(false);
}

void NetworkingManagerServer::RecievePackage()
{
	sf::Packet packet;
	sf::IpAddress clientAddress;
	unsigned short clientPort;

	if (socket->receive(packet, clientAddress, clientPort) == sf::Socket::Done)
	{
		std::string message;
		packet >> message;

		std::cout << "Received message from " << clientAddress << ":" << clientPort << ": " << message << std::endl;
	}
}
