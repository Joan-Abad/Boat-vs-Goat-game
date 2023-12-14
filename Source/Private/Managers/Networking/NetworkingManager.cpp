#include "Managers/Networking/NetworkingManager.h"
#include <iostream>

unsigned short NetworkingManager::serverGamePort = 53001;
const std::string NetworkingManager::accessKey = "bWantsToPlay";
const std::string NetworkingManager::connectionWithServer = "bAcceptedByServer";
const std::string NetworkingManager::startGameKey = "bStartGame";
const std::string NetworkingManager::numPlayers = "numPlayers";
const const char* NetworkingManager::key_PlayerID = "playerID";


NetworkingManager::NetworkingManager() : bIsServer(false), gameObjectsData(Json::arrayValue), mapNetData(Json::arrayValue)
{

}

void NetworkingManager::AddPacketHeader()
{
	rootData[key_PlayerID] = playerID;
}

void NetworkingManager::ClearNetData()
{
	gameObjectsData.clear();
	mapNetData.clear();
	rootData.clear();
}