#include "Map/Map.h"
#include <iostream>
#include "Window.h"

void Map::AddPlayer(Player player)
{
	players.emplace_back(player);
}
