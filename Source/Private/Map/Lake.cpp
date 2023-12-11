#include "Map/Lake.h"
#include <iostream>
#include "Window.h"
#include "Managers/GameManager.h"
#include "GameObjects/Players/Boat.h"
#include "Managers/TextureManager.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"

Map_Lake::Map_Lake()
{
	TextureManager& TM = TextureManager::GetTextureManager();

	TM.AddTexture(backgroundLakePath);
	TM.AddTexture(boat1Path);
	TM.AddTexture(boat2Path);
	TM.AddTexture(bulletPath);

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
	bool playerLocallyControlled = false;
	const unsigned short playerID = AppManager::GetAppManager()->GetNetworkManager()->GetPlayerID();
	
	for (int i = 0; i < playersQuantity; i++)
	{

		if (playerID == i)
			playerLocallyControlled = true;
		else
			playerLocallyControlled = false;
		
		Boat* newBoat = nullptr; 

		if(i == 0)
		{
			newBoat = new Boat(playerLocallyControlled, PlayerInitialInfo(sf::Vector2f(WINDOW_SIZE.x * 0.15f, WINDOW_SIZE.y * 0.5f), 90.f));
			AddPlayer(*newBoat);
		}
		else if (i == 1)
		{
			newBoat = new Boat(playerLocallyControlled, PlayerInitialInfo(sf::Vector2f(WINDOW_SIZE.x * 0.85f, WINDOW_SIZE.y * 0.5f), -90.f));
			AddPlayer(*newBoat);
		}
		else if (i == 2)
		{
			newBoat = new Boat(playerLocallyControlled, PlayerInitialInfo(sf::Vector2f(WINDOW_SIZE.x * 0.5f, WINDOW_SIZE.y * 0.1f), -180.f));
			AddPlayer(*newBoat);
		}

		if (newBoat)
			newBoat->Init();
	}
}

void Map_Lake::UpdateMap()
{
	Map::UpdateMap();
}

void Map_Lake::DrawWap(Window& window)
{
	sf::RenderWindow& sfmlWindow = window.GetWindow();
	
	window.ClearWindow();

	sfmlWindow.draw(backgroundSprite);

	for (auto& go : levelGameObjects)
	{
		go->Draw(sfmlWindow);
	}

	window.Display();
}

void Map_Lake::EndMap()
{
}
