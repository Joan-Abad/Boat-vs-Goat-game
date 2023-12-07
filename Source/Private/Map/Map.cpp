#include "Map/Map.h"
#include <iostream>
#include "Window.h"

void Map::UpdateMap()
{
	for (auto& player : players)
	{
		player->HandlePlayerInput();
		player->Update();
	}
}

void Map::AddPlayer(Player& player)
{
	players.emplace_back(&player);
}
