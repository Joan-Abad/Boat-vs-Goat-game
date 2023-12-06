#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class NetworkingManager; 
class Map; 
class Window; 


#define WINDOW_NAME "BOAT VS GOAT"
#define WINDOW_SIZE sf::Vector2f(720,480)

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
	static GameManager* GetGameManager()
	{
		if (!instance)
			instance = new GameManager();

		return instance; 
	}

	//Init the application asking the player if wants to be the server or client
	void InitGame();
	void Update();
	void CloseGame();
	
	void SetStartGame(bool bDrawGame);
private: 
	GameManager();

	//Handles all network data that needs to be processed by the application
	NetworkingManager* NetworkManager; 

	//Stores if the player is server or client
	EPlayerType playerType; 

	//Handles the lifetime of the applcation
	bool bCloseGame = false; 

	bool bDrawGame = false; 

	//The current Map displayed on the window
	Map* currentGameMap; 

	Window* window; 

	static GameManager* instance; 
};