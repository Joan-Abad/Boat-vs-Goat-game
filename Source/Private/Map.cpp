#include "Map.h"
#include <iostream>
#include "Window.h"

Map::Map(const char* backgroundPath)
{
	if (!texture.loadFromFile(backgroundPath))
		std::cerr << "BackgroundPath error\n";

	backgroundSprite.setTexture(texture);
}

void Map::DrawWap(Window& window)
{
	window.ClearWindow();
	window.GetWindow().draw(backgroundSprite);
	window.Display();
}
