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

#define LAN 0

class NetworkingManager
{
	friend class AppManager; 
public:

	virtual ~NetworkingManager() = default;
	
	virtual void OnInit() = 0;

	//This function should be the one that handles how the data is send.
	virtual void UpdateNetworkData() = 0;

	NetworkingManager(const NetworkingManager&) = delete;
	NetworkingManager& operator=(const NetworkingManager&) = delete;
	
	//GETTERS
	inline const static unsigned short GetGamePort() { return gamePort; }; 
	inline const unsigned short GetPlayerID() { return playerID; };

protected:
	NetworkingManager() = default;

	sf::UdpSocket udpSocket; 
	std::vector<sf::Packet> packetsToSendThisFrame;
	//The plyer id associated with the application player
	unsigned short playerID; 
	
	//Communication keys
	const static std::string accessKey;
	const static std::string connectionWithServer; 
	const static std::string startGameKey;
	const static std::string numPlayers;
	const static std::string key_PlayerID;
	
	static unsigned short gamePort;
}; 
