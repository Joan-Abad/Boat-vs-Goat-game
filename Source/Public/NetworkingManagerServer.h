#pragma once
#include "NetworkingManager.h"

class NetworkingManagerServer : public NetworkingManager
{
public: 
	NetworkingManagerServer();
protected: 

	virtual void Bind(int port) override;
	virtual void RecievePackage() override;
};