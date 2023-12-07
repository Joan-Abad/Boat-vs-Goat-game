#include "Map/Lake.h"
#include <iostream>
#include "Window.h"

Map_Lake::Map_Lake()
{
	if (!backgroundTexture.loadFromFile(backgroundLakePath))
		std::cerr << "ERROR LOADING LAKE TEXTURE\n";

	backgroundSprite.setTexture(backgroundTexture);
}

void Map_Lake::InitMap(int playersQuantity)
{
	for (int i = 0; i < playersQuantity; i++)
	{
		//Init new boat player
		//Player player;
		//players.push_back(player);
	}
}

void Map_Lake::UpdateMap()
{

}

void Map_Lake::DrawWap(Window& window)
{
	sf::RenderWindow& sfmlWindow = window.GetWindow();
	
	window.ClearWindow();

	sfmlWindow.draw(backgroundSprite);

	window.Display();
}

void Map_Lake::EndMap()
{
}
