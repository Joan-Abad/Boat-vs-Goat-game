#include "Map.h"
#include <iostream>

Map::Map(const char* backgroundPath)
{
	if (!texture.loadFromFile(backgroundPath))
		std::cerr << "BackgroundPath error\n";

	backgroundSprite.setTexture(texture);
}

void Map::DrawWap(sf::RenderWindow& window)
{
	window.draw(backgroundSprite);
}
