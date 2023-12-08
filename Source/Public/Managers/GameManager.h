#pragma once
#include <vector>
#include <unordered_map>
#include <string>

class Map; 
class Window; 

#define WINDOW_NAME "BOAT VS GOAT"
#define WINDOW_SIZE sf::Vector2f(1920 , 1080)

class GameManager
{
	friend class NetworkingManager;
	friend class AppManager; 
public:
	static GameManager* GetGameManager()
	{
		if (!instance)
			instance = new GameManager();

		return instance; 
	}

	~GameManager();
	
	void InitGameWindow();
	void InitGameMap(Map* map, unsigned short playerQuantity);
	void Update();
	void AddGameMap(const std::string& mapName, Map* map);

	const std::string LakeMap = "Lake";

	//GETTERS
	Map* GetMap(std::string MapName);
	Map* GetCurrentMap() { return currentGameMap; };
	Window* GetGameWindow() { return window; };
private: 
	GameManager();

	//string = name of the map
	//Map = current game map
	std::unordered_map<std::string, Map*> gameMaps;

	//The current Map displayed on the window
	Map* currentGameMap;

	//Window of the game
	Window* window;

	//Not owning pointer to the Network Manager
	NetworkingManager* networkManager; 

	bool bHasGameStarted;

	static GameManager* instance; 
};