#include "Managers/GameManager.h"
#include "Map/Map.h"
#include "Map/Lake.h"
#include "Window.h"
#include "Input/InputManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : bHasGameStarted(false)
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
	window->GetWindow().setFramerateLimit(60);
	bHasGameStarted = true; 
}

void GameManager::InitGameMap(Map* map, unsigned short playerQuantity)
{
	currentGameMap = map;
	currentGameMap->InitMap(*window, playerQuantity);

}

void GameManager::Update(float DeltaTime)
{
	//Draw the game
	if (currentGameMap && window)
	{
		currentGameMap->UpdateMap(DeltaTime);
		currentGameMap->DrawWap(*window);
	}
}

void GameManager::AddGameMap(const std::string &mapName, Map* map)
{
	gameMaps[mapName] = map;
}

Map* GameManager::GetMap(std::string MapName)
{
	return gameMaps[MapName];
}
