#include "Map/Lake.h"
#include <iostream>
#include "Window.h"
#include "Managers/GameManager.h"
#include "GameObjects/Players/Boat.h"
#include "Managers/TextureManager.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/SoundManager.h"
#include "Managers/FontManager.h"
#include "Input/InputManager.h"

Map_Lake::Map_Lake() : gameOver(false)
{
	TextureManager& TM = TextureManager::GetTextureManager();

	TM.AddTexture(backgroundLakePath);
	TM.AddTexture(boat1Path);
	TM.AddTexture(boat2Path);
	TM.AddTexture(bulletPath);
	TM.AddTexture(heartPath);
	TM.AddTexture(goatPath);

	FontManager& fontManager = *FontManager::Get();

	fontManager.AddFont(lakeFontPath);
	
	SoundManager* SM = SoundManager::Get();
	if (SM)
	{
		SM->CreateSound("Sound/Boat/blaster.wav");
		SM->CreateSound("Sound/winSound.wav");
	}

	backgroundSprite.setTexture(*TM.GetTexture(backgroundLakePath));
	goatSprite.setTexture(*TM.GetTexture(goatPath));
	goatSprite.setOrigin(goatSprite.getLocalBounds().width / 2, goatSprite.getLocalBounds().height / 2);
	goatSprite.setPosition(WINDOW_SIZE.x/2, WINDOW_SIZE.y/2);
	
	sf::Vector2f winningPosition = { goatSprite.getPosition().x - goatSprite.getTexture()->getSize().x / 3, goatSprite.getPosition().y - goatSprite.getTexture()->getSize().y / 2 };
	winningText.setString("NULL");
	winningText.setOutlineColor(sf::Color::Black);
	winningText.setCharacterSize(48.f);
	winningText.setOutlineThickness(4.f);
	winningText.setFont(*fontManager.GetFont(lakeFontPath));
	winningText.setPosition(winningPosition);
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
		sf::Vector2f scale = { 0.1f, 0.1f };
		//Should use spawn actor, not new
		if(i == 0)
		{
			newBoat = new Boat(GameObjectInitialInfo(sf::Vector2f(WINDOW_SIZE.x * 0.15f, WINDOW_SIZE.y * 0.5f), 90.f, scale), playerLocallyControlled);
		}
		else if (i == 1)
		{
			newBoat = new Boat(GameObjectInitialInfo(sf::Vector2f(WINDOW_SIZE.x * 0.85f, WINDOW_SIZE.y * 0.5f), -90.f, scale), playerLocallyControlled);
		}
		else if (i == 2)
		{
			newBoat = new Boat(GameObjectInitialInfo(sf::Vector2f(WINDOW_SIZE.x * 0.5f, WINDOW_SIZE.y * 0.1f), -180.f, scale), playerLocallyControlled);
		}

		if (newBoat)
		{
			newBoat->Init();
			AddPlayer(newBoat->GetGameObjectID(), *newBoat);
		}
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
		go.second->Draw(sfmlWindow);
	}

	if (gameOver)
	{
		sfmlWindow.draw(goatSprite);
		sfmlWindow.draw(winningText);
	}

	window.Display();
}

bool Map_Lake::CheckWinCondition()
{
	int numberBoatsAlive = 0; 
	Boat* winningBoat = nullptr;
	for (int i = 0; i < players.size(); i++)
	{
		Boat* boat= dynamic_cast<Boat*>(players[i]);
		if (boat)
		{
			if (boat->GetLifes() > 0)
			{
				numberBoatsAlive++;
				winningBoat = boat; 
			}
		}
	}

	//More than one boat alive
	if (numberBoatsAlive > 1)
		return false;
	
	std::string playerText = "Player" + std::to_string(winningBoat->GetPlayerID() + 1) + " WON ";
	FinishMap(playerText.c_str());
	AddLocalNetworkDataToSend(key_win, playerText.c_str());

	return true;
}

void Map_Lake::EndMap()
{
	AddGameObjectNetDataToManagerNetData();
}

void Map_Lake::UpdateClientNetData(const Json::Value& root)
{
	Json::StreamWriterBuilder writerBuilder;
	std::string mapDataString = Json::writeString(writerBuilder, root);

	if (root.isMember(key_win))
	{
		const char* playerWon = root[key_win].asCString();
		FinishMap(playerWon);
	}

}

void Map_Lake::FinishMap(const char* playerName)
{
	gameOver = true;
	std::string strMsg = playerName;
	winningText.setString(strMsg);

	for (auto& player : players)
	{
		Boat* boat = dynamic_cast<Boat*>(player);
		boat->DisableBoat();
	}

	Sound* sound = SoundManager::Get()->GetSound("Sound/winSound.wav");
	if (sound)
		sound->PlaySound();

	InputManager::GetInputManager()->EnableInput(false);
}
