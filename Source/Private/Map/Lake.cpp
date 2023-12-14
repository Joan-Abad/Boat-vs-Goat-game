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
#include <random>
#include <ApplicationHelper.h>
#include "GameObjects/Cannon.h"
#include "GameObjects/Bullet.h"

Map_Lake::Map_Lake() : gameOver(false), spawnTimeMissileMin(0.8f), spawnTimeMissileMax(2.f)
{
	TextureManager& TM = TextureManager::GetTextureManager();

	TM.AddTexture(backgroundLakePath);
	TM.AddTexture(boat1Path);
	TM.AddTexture(boat2Path);
	TM.AddTexture(bulletPath);
	TM.AddTexture(heartPath);
	TM.AddTexture(goatPath);
	TM.AddTexture(missilePath);

	FontManager& fontManager = *FontManager::Get();

	fontManager.AddFont(lakeFontPath);
	
	SoundManager* SM = SoundManager::Get();
	if (SM)
	{
		Cannon::cannonSound = SM->CreateSound("Sound/Boat/Cannon.wav");
		SM->CreateSound("Sound/winSound.wav");
		SM->CreateSound("Sound/PirateMusic.wav");
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

	//Init missils game object pool
	for (int i = 0; i < MaxMissilsOnScreen; i++)
	{
		//Spawn and hide bullets
		Bullet* missile = SpawnGameObject<Bullet>(GameObjectInitialInfo());
		missile->bTickEnabled = false;
		missile->HideGameObject();
		missile->objectCollision = CollisionChannels::NoCollision;
		missiles[i] = missile;
	}
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
	const unsigned short playerID = AppManager::GetNetworkManager()->GetPlayerID();
	
	timer.restart();
	SetNewSpawnTimeMissle();

	SpawnMapCannons();

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

	SoundManager* SM = SoundManager::Get();
	if (SM)
	{
		Sound* sound = SM->GetSound("Sound/PirateMusic.wav");
		sound->PlaySound(true);
		sound->SetVolume(50.f);
	}
}

void Map_Lake::UpdateMap(float DeltaTime)
{
	Map::UpdateMap(DeltaTime);

	if (AppManager::GetNetworkManager()->GetIsServer() && !gameOver)
	{
		sf::Time elapsedTime = timer.getElapsedTime();

		if (elapsedTime.asSeconds() >= spawnTimeMissileMin)
		{
			SpawnMissile();
			SetNewSpawnTimeMissle();
			timer.restart();
		}
	}

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

void Map_Lake::SetNewSpawnTimeMissle()
{
	finalSpawnTimeMissile = ApplicationHelper::GetRandomValue<float>(spawnTimeMissileMin, spawnTimeMissileMax);
}

void Map_Lake::SpawnMapCannons()
{
	//Top screeen, bot screen, right screen and left screen
	int cannonPerSide = 4;

	//Space between cannons -> 640
	const int cannonPositionDiffX = WINDOW_SIZE.x / cannonPerSide;
	const int cannonPositionDiffY = WINDOW_SIZE.y / cannonPerSide;
	totalCannons = cannonPerSide * 4; // 4 is the window sides as it will be spawning from top, bottom, left, right

	//First cannon posttion -> 320
	const sf::Vector2i initialCannonPosition = { cannonPositionDiffX / 2 , cannonPositionDiffY / 2 };
	int sideSpawnTracker = 1;
	int changeSideNum = cannonPerSide;
	sf::Vector2i icpTracker = initialCannonPosition;

	sf::Vector2f cannonTextureSize = static_cast<sf::Vector2f>(TextureManager::GetTextureManager().GetTexture(missilePath)->getSize()) ;
	cannonTextureSize *= 0.2f;
	std::vector<sf::Vector2i> positions;
	int half = totalCannons / 2;

	//Calculate cannon posiitons
	for (int i = 0; i < totalCannons; i++)
	{
		float variantPosition = 0;
		Cannon* missile;
		//del 0 al 5
		if (i < half)
		{
			//Down
			if (i % 2 != 0)
			{
				variantPosition = WINDOW_SIZE.y; // cannon texture size
				//320 + 640 = 960
				missile = SpawnGameObject<Cannon>(GameObjectInitialInfo(sf::Vector2f(icpTracker.x, variantPosition), 45.f, sf::Vector2f(1.0f, 1.0f)));
				missile->SetPosition(sf::Vector2f(icpTracker.x, variantPosition));
				missile->bTickEnabled = false;

				positions.push_back(sf::Vector2i(icpTracker.x, variantPosition));
				icpTracker.x += cannonPositionDiffX;
			}
			//Top
			else
			{
				missile = SpawnGameObject<Cannon>(GameObjectInitialInfo(sf::Vector2f(icpTracker.x, variantPosition), 45.f, sf::Vector2f(1.0f, 1.0f)));
				missile->SetPosition(sf::Vector2f(icpTracker.x, variantPosition));
				missile->SetRotation(180.f);
				positions.push_back(sf::Vector2i(icpTracker.x, variantPosition));
			}

		}
		//del 6 al 11
		else
		{
			//Right
			if (i % 2 != 0)
			{
				variantPosition = WINDOW_SIZE.x;
				missile = SpawnGameObject<Cannon>(GameObjectInitialInfo(sf::Vector2f(variantPosition, icpTracker.y), 45.f, sf::Vector2f(1.0f, 1.0f)));
				missile->SetPosition(sf::Vector2f(variantPosition, icpTracker.y));
				missile->SetRotation(-90.f);
				icpTracker.y += cannonPositionDiffY;
			}
			//Left
			else
			{
				missile = SpawnGameObject<Cannon>(GameObjectInitialInfo(sf::Vector2f(variantPosition, icpTracker.y), 45.f, sf::Vector2f(1.0f, 1.0f)));
				missile->SetPosition(sf::Vector2f(variantPosition, icpTracker.y));
				missile->SetRotation(90.f);
				positions.push_back(sf::Vector2i(variantPosition, icpTracker.y));
			}

		}
		cannons.push_back(missile);
	}

	int i = 0;
}

void Map_Lake::SpawnMissile()
{	
	int cannonID = ApplicationHelper::GetRandomValue<float>(0,totalCannons-1);
	
	Cannon* cannon = cannons[cannonID];
	cannon->ThrowCannonBall(*missiles[missileTracker]);

	//End Replication Code
	if (missileTracker == MaxMissilsOnScreen - 1)
		missileTracker = 0; 

	missileTracker++;
}

void Map_Lake::FinishMap(const char* playerName)
{
	gameOver = true;
	std::string strMsg = playerName;
	winningText.setString(strMsg);

	SoundManager* SM = SoundManager::Get();
	SM->GetSound("Sound/PirateMusic.wav")->StopSound();

	for (auto& player : players)
	{
		Boat* boat = dynamic_cast<Boat*>(player);
		boat->DisableBoat();
	}

	for (auto& go : levelGameObjects)
	{
		go.second->bTickEnabled = false; 
	}

	Sound* sound = SoundManager::Get()->GetSound("Sound/winSound.wav");
	if (sound)
		sound->PlaySound();

	InputManager::GetInputManager()->EnableInput(false);
}
