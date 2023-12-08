#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class NetworkingManager; 
class Map; 
class Window; 
class GameManager;


#if _DEBUG
#define FORCE_SERVER_IP 0
#define SERVER_IP "192.168.0.15"
#endif



//Class that handles all the Application
class AppManager
{
	friend class GameManager; 
public: 
	static AppManager* GetAppManager()
	{
		if (!instance)
			instance = new AppManager();

		return instance; 
	}

	//Init the application asking the player if wants to be the server or client
	void InitGame();
	void Update();
	void CloseGame();
	~AppManager();
	NetworkingManager* GetNetworkManager() { return NetworkManager; };

private: 
	AppManager();

	//Handles all network data that needs to be processed by the application
	NetworkingManager* NetworkManager; 

	//Handles all related to the game
	GameManager* gameManager; 

	//Handles the lifetime of the applcation
	bool bCloseGame = false; 

	static AppManager* instance; 
};