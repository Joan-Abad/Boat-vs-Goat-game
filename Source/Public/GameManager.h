#pragma once
#include <vector>
#include <unordered_map>
#include <string>

class Map; 
class Window; 

#define WINDOW_NAME "BOAT VS GOAT"
#define WINDOW_SIZE sf::Vector2f(720,480)

class GameManager
{
public:
	static GameManager* GetGameManager()
	{
		if (!instance)
			instance = new GameManager();

		return instance; 
	}

	~GameManager();
	
	void InitGame();
	void Update();
	void AddGameMap(const std::string& mapName, Map* map);
	Map* GetMap(std::string MapName);
private: 
	GameManager();

	//string = name of the map
	//Map = current game map
	std::unordered_map<std::string, Map*> gameMaps;

	//The current Map displayed on the window
	Map* currentGameMap;

	//Window of the game
	Window* window;

	const std::string LakeMap = "Lake";

	static GameManager* instance; 
};