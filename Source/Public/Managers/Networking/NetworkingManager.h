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
	inline bool GetIsServer() { return bIsServer; };

	//Adds the data that will be send at the end of the frame
	//Value needs to be a supported JSON type (string, number, Json Object, array, bool or null)
	template <typename Value>
	void AddNetworkDataToSend(const char* KEY, Value valueToSend)
	{
		GetRootData()[KEY] = valueToSend;
	}


protected:
	NetworkingManager();

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
	const static std::string fnc_AccelerateBoat; 
	
	static unsigned short gamePort;

	//Clears all the json data. Initialy when a packet is send, this should be called
	void ClearRootData();
	//Gets the json string that is used to send data to server/client
	inline Json::Value &GetRootData() { return rootData; };
private: 
	//Value to write that will be send each frame if contains any information
	Json::Value rootData;

	//Checks if this process is a server 
	bool bIsServer;
}; 
