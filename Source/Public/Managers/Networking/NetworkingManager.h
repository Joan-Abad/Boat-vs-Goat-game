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

	//Gets the json string that is used to send data to server/client
	inline Json::Value& GetRootData() { return rootData; };
	inline Json::Value& GetMapNetData() { return mapNetData; };
	inline Json::Value& GetGameObjectsNetData() { return gameObjectsData; };
	//Communication keys
	const static std::string accessKey;
	const static std::string connectionWithServer;
	const static std::string startGameKey;
	const static std::string numPlayers;
	const static const char* key_PlayerID;
	const static std::string fnc_AccelerateBoat;

protected:
	NetworkingManager();

	sf::UdpSocket udpSocket; 
	std::vector<sf::Packet> packetsToSendThisFrame;
	//The plyer id associated with the application player
	unsigned short playerID; 

	static unsigned short gamePort;

	//Adds the packet header for the server to be able to read it
	void AddPacketHeader();

	//Clears all the json data. Initialy when a packet is send, this should be called
	void ClearNetData();

private: 
	//Value to write that will be send each frame if contains any information
	Json::Value rootData;
	Json::Value mapNetData; 
	Json::Value gameObjectsData;

	//Checks if this process is a server 
	bool bIsServer;
}; 
