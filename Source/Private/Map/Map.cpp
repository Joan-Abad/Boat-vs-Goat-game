#include "Map/Map.h"
#include <iostream>
#include "Window.h"

Map::~Map()
{
	for (auto& levelGameObject : levelGameObjects)
		delete levelGameObject.second;

}

void Map::UpdateMap()
{
	int numGameObjects = levelGameObjects.size(); 

	for (auto& goEntry: levelGameObjects)
	{
		GameObject& go = *goEntry.second;

		if (go.bTickEnabled)
		{
			go.Update();
			go.EndUpdate();
		}
	}
}

void Map::AddDataToSendServer()
{
	
}

void Map::AddPlayer(int playerID, Player& player)
{
	players.emplace_back(&player);
	levelGameObjects[playerID] = &player;
}
