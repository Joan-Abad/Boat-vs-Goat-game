#pragma once
#include "NetworkingManager.h"

class NetworkingManagerClient : public NetworkingManager
{
public: 
	NetworkingManagerClient();

protected:

	virtual void Bind(int port) override;
	virtual void RecievePackage() override;
	virtual void SendPackages();

private: 
	sf::IpAddress ServerAddress; 
};