#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <json.h>
#include "Managers/GameManager.h"
#include "Managers/AppManager.h"

#define LAN 1

class NetworkingManager
{
public:

	virtual ~NetworkingManager() = default;
	
	virtual void OnInit() = 0;

	//This function should be the one that handles how the data is send.
	virtual void UpdateNetworkData() = 0;

	NetworkingManager(const NetworkingManager&) = delete;
	NetworkingManager& operator=(const NetworkingManager&) = delete;
	const static unsigned short GetGamePort() { return gamePort; };
protected:
	NetworkingManager() = default;

	sf::UdpSocket udpSocket; 
	std::vector<sf::Packet> packetsToSendThisFrame;
	
	//Communication keys
	const static std::string accessKey;
	const static std::string connectionWithServer; 
	const static std::string startGameKey;
	
	static unsigned short gamePort;
}; 
