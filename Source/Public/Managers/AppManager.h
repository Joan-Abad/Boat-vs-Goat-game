#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>

class NetworkingManager; 
class Map; 
class Window; 
class GameManager;

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

	//Calls all the update of the game
	void Update();

	//When we want to close the application, we should call this function
	void CloseGame();

	//Starts to check for game Input
	void InitGameInput();

	//Destructor
	~AppManager();

	//Gets the NetworkManager
	static NetworkingManager* GetNetworkManager() { return instance->NetworkManager; };
	inline bool GetIsGameClosed() { return bGameClosed; };
private: 
	AppManager();

	//Handles all network data that needs to be processed by the application
	NetworkingManager* NetworkManager; 

	//Handles all related to the game
	GameManager* gameManager; 

	std::thread inputThread;

	//Handles the lifetime of the applcation
	bool bGameClosed = false; 

	//singleton instance of the app manager
	static AppManager* instance; 
};