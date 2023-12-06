#include "GameManager.h"
#include "Map.h"
#include "Window.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	AddGameMap(LakeMap, new Map("Art/Background/Water.jpg"));

	currentGameMap = GetMap(LakeMap);
}

GameManager::~GameManager()
{
	//delete all maps
	for (auto& map : gameMaps)
	{
		delete map.second; 
	}
}

void GameManager::InitGame()
{
	window = new Window(WINDOW_NAME, WINDOW_SIZE);
}

void GameManager::Update()
{
	sf::Event event;
	if (window)
	{
		while (window->GetWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->GetWindow().close();
		}
	}

	//Draw the game
	if (currentGameMap && window)
		currentGameMap->DrawWap(*window);
}

void GameManager::AddGameMap(const std::string &mapName, Map* map)
{
	gameMaps[mapName] = map;
}

Map* GameManager::GetMap(std::string MapName)
{
	return gameMaps[MapName];
}
