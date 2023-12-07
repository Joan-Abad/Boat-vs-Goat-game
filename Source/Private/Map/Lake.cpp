#include "Map/Lake.h"
#include <iostream>
#include "Window.h"
#include "Managers/GameManager.h"
#include "Player/Boat.h"
#include "Managers/TextureManager.h"

Map_Lake::Map_Lake()
{
	TextureManager& TM = TextureManager::GetTextureManager();

	TM.AddTexture(backgroundLakePath);
	TM.AddTexture(boat1Path);
	TM.AddTexture(boat2Path);

	backgroundSprite.setTexture(*TM.GetTexture(backgroundLakePath));
}

Map_Lake::~Map_Lake()
{
	TextureManager& TM = TextureManager::GetTextureManager();

	TM.RemoveTexture(backgroundLakePath);
	TM.RemoveTexture(boat1Path);
	TM.RemoveTexture(boat2Path);
}


void Map_Lake::InitMap(Window& window, int playersQuantity)
{
	for (int i = 0; i < playersQuantity; i++)
	{
		if(i == 0)
		{
			Boat player(window.GetWindow(), false, PlayerInitialInfo(i, sf::Vector2f(WINDOW_SIZE.x * 0.15f, WINDOW_SIZE.y * 0.5f), -90.f, boat1Path));
			AddPlayer(std::move(player));
		}
		else if (i == 1)
		{
			Boat player(window.GetWindow(), false, PlayerInitialInfo(i, sf::Vector2f(WINDOW_SIZE.x * 0.85f, WINDOW_SIZE.y * 0.5f), 90.f, boat2Path));
			AddPlayer(std::move(player));
		}
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

	for (auto& boat : players)
	{
		boat.Draw(sfmlWindow);
	}

	window.Display();
}

void Map_Lake::EndMap()
{
}
