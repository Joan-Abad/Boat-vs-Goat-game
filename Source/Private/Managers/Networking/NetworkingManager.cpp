#include "Managers/Networking/NetworkingManager.h"
#include <iostream>

unsigned short NetworkingManager::gamePort = 53001;
const std::string NetworkingManager::accessKey = "bWantsToPlay";
const std::string NetworkingManager::connectionWithServer = "bAcceptedByServer";
const std::string NetworkingManager::startGameKey = "bStartGame";
const std::string NetworkingManager::numPlayers = "numPlayers";
const std::string NetworkingManager::key_PlayerID = "playerID";


NetworkingManager::NetworkingManager() : bIsServer(false)
{

}

void NetworkingManager::ClearRootData()
{
	rootData.clear();
}