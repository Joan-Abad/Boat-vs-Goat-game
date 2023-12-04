#include "NetworkingManagerClient.h"
#include <iostream>

NetworkingManagerClient::NetworkingManagerClient()
{
	std::cout << "PLAYING ON Client\n";
	ServerAddress = "192.168.0.15";
}

void NetworkingManagerClient::Bind(int port)
{

}

void NetworkingManagerClient::RecievePackage()
{
}

void NetworkingManagerClient::SendPackages()
{
	for (auto& packetToSend : packetsToSend)
	{

		sf::Socket::Status status = socket->send(packetToSend, ServerAddress, port);
		std::cout << "Sending packet";

	}
	packetsToSend.clear();
}

