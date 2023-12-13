#pragma once
#include "NetworkingManager.h"

enum class EClientManagementData
{
	EConnectingToServer,
	EWaitingForPlayers,
	EStartingTheMatch,
	EPlayMatch,
	EEndMatch
};

class NetworkingManagerClient : public NetworkingManager
{
public:
	NetworkingManagerClient(const sf::IpAddress& ipAddress, const unsigned int port);

	virtual void OnInit() override;

	virtual void UpdateNetworkData() override;

protected: 
	virtual void EndMatch() override;

private:
	//Sets the packet to the server to be able to join the multiplayer game
	void SetInitialPacketToServer();
	
	//Waits for the server confirmation to join the game
	void WaitForServerResponse();

	//Waits for the server to start playing the game
	void WaitForGameStart();

	void RecieveDataFromServer();
	void SendGameDataToServer();
	//ip address of the server
	sf::IpAddress serverAddress;

	//Bool used for displaying once the waiting msg
	bool bDisplayMessageWaitingOtherPlayers = true;
	//Client port
	static unsigned short processPort;
	static unsigned short serverPort; 

	//Manages the network data of the game from the client side
	EClientManagementData clientManagementData;
};