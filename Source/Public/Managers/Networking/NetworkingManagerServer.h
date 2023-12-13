#pragma once
#include "NetworkingManager.h"

struct PlayerConnectionInfo
{
	PlayerConnectionInfo() = default;

	PlayerConnectionInfo(sf::IpAddress ip, int port)
	{
		ipAddress = ip;
		this->port = port;
	}

	sf::IpAddress ipAddress;
	int port;
};

enum class EServerManagementData
{
	EWaitingPlayers,
	EStartingTheMatch,
	EStartTheMatch,
	EPlayMatch,
	EEndMatch
};

class NetworkingManagerServer : public NetworkingManager
{
public:
	NetworkingManagerServer();

	virtual void OnInit() override;

	virtual void UpdateNetworkData() override;

protected:
	virtual void EndMatch() override;
	
private:
	//Code that waits for clients to connect and pass througt the Strarting Match state
	void WaitForClientsToConnect();

	//When all data flags are recieved from players that are ready, send a flag to start the match! 
	void StartGameServerAndClients();

	void SendGameDataToClients();
	void RecieveGameDataFromClients();

	//This array holds the info to recieve and send information of the players
	std::unordered_map<std::string, PlayerConnectionInfo> players;

	//Variable that manages the network data of the game from the server side
	EServerManagementData serverManagementData;

	//Number of players needed to start the game
	int numPlayersToStartTheGame;

	bool bDisplayMessageWaitingForClients = true;
};