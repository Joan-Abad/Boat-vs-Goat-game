#include "Managers/GameManager.h"
#include "Map/Map.h"
#include "Map/Lake.h"
#include "Window.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	AddGameMap(LakeMap, new Map_Lake());
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

void GameManager::InitGameWindow()
{
	window = new Window(WINDOW_NAME, WINDOW_SIZE);
}

void GameManager::InitGameMap(Map* map, unsigned short playerQuantity)
{
	currentGameMap = map;
	currentGameMap->InitMap(*window, playerQuantity);
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
