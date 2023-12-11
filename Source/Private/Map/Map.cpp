#include "Map/Map.h"
#include <iostream>
#include "Window.h"

Map::~Map()
{
	for (auto& levelGameObject : levelGameObjects)
		delete levelGameObject;

}

void Map::UpdateMap()
{
	int numGameObjects = levelGameObjects.size(); 
	for (int i = 0; i < numGameObjects; i++)
	{
		if(levelGameObjects[i]->bTickEnabled)
			levelGameObjects[i]->Update();

		levelGameObjects[i]->EndUpdate();
	}
}

void Map::AddDataToSendServer()
{
	
}

void Map::AddPlayer(Player& player)
{
	players.emplace_back(&player);
	levelGameObjects.push_back(&player);
}
