#include "Map/Map.h"
#include <iostream>
#include "Window.h"

Map::~Map()
{
	for (auto& player : players)
		delete player;
}

void Map::UpdateMap()
{
	for (auto& player : players)
	{
		player->HandlePlayerInput();
		player->Update();
	}
}

void Map::AddDataToSendServer()
{

}

void Map::AddPlayer(Player& player)
{
	players.emplace_back(&player);
}
