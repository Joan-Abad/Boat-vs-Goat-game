#pragma once
#include <iostream>

class NetworkingManager; 

#if _DEBUG
#define FORCE_SERVER_IP 0
#define SERVER_IP "192.168.0.15"
#endif

enum class EPlayerType
{
	EServer,
	EClient,
	ENotAssigned
};

class GameManager
{
public: 
	GameManager();
	
	//Init the application asking the player if wants to be the server or client
	void InitGame();
	void Update();
	void CloseGame();
private: 
	NetworkingManager* NetworkManager; 

	EPlayerType playerType; 

	bool bCloseGame = false; 




};