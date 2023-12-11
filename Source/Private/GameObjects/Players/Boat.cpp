#include "GameObjects/Players/Boat.h"
#include <iostream>
#include "ApplicationHelper.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Input/InputManager.h"
#include "Managers/TextureManager.h"
#include "Map/Map.h"
#include "Managers/SoundManager.h"

const char* Boat::key_AccelerateBoatID = "AccelerateBoatID";
const char* Boat::key_RotateBoatLeftID = "RotateBoatLeftID";
const char* Boat::key_RotateBoatRightID = "RotateBoatRightID";
const char* Boat::key_boatPosition = "boatPosition";
const char* Boat::key_boatAngle = "boatAngle";
const char* Boat::key_boatID= "boatID";
const char* Boat::key_ShootBoatID = "shootBoat";
const char* Boat::key_SpawnBullet = "spawnBullet";

unsigned short Boat::boatCounter = 0; 

Boat::Boat(bool PlayerPlayable, PlayerInitialInfo playerInitialInfo) : Player(PlayerPlayable, playerInitialInfo), angleBoatSpeedEachSecond(360.f), 
bIsBoatAccelerating (false), bIsBoatRotatingLeft(false), bIsBoatRotatingRight(false), shootingCD(0.25f), bulletTracker(0)
{

	//Graphics part
	if (boatCounter == 0)
		initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture(PLAYER1TEXTPATH));
	else /*(boatCounter == 1)*/
		initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture(PLAYER2TEXTPATH));

	initialSprite.setOrigin(initialSprite.getLocalBounds().width / 2, initialSprite.getLocalBounds().height / 2);
	//End Graphic part

	//Sound part
	shootingSound = SoundManager::Get()->GetSound("Sound/Boat/blaster.wav");
	//End sound part
	
	Map* map = GetCurrentMap();

	for (int i = 0; i < MaxBulletsPerBoatOnScreen; i++)
	{
		//Spawn and hide bullets
		Bullet* bullet = map->SpawnGameObject<Bullet>(GameObjectInitialInfo());
		bullet->bTickEnabled = false; 
		bullet->HideGameObject();
		bullets[i] = bullet;
	}
	

	boatCounter++;


	if (PlayerPlayable)
	{
		InputManager* im = InputManager::GetInputManager();
		//Get input actions references
		action_W = im->GetInputAction(sf::Keyboard::W);
		action_RotateLeft = im->GetInputAction(sf::Keyboard::Key::Q);
		action_RotateRight = im->GetInputAction(sf::Keyboard::Key::E);
		action_ShootBoat = im->GetInputAction(sf::Keyboard::Key::Space);

		//Accelerate Boat
		action_W->OnKeyTriggered.push_back(BindAction(&Boat::StartAccelerateBoat, this));
		action_W->OnKeyReleased.push_back(BindAction(&Boat::StopAccelerateBoat, this));

		//Rotate left
		action_RotateLeft->OnKeyTriggered.push_back(BindAction(&Boat::StartRotateBoatLeft, this));
		action_RotateLeft->OnKeyReleased.push_back(BindAction(&Boat::StopRotateBoatLeft, this));

		//Rotate Right
		action_RotateRight->OnKeyTriggered.push_back(BindAction(&Boat::StartRotateBoatRight, this));
		action_RotateRight->OnKeyReleased.push_back(BindAction(&Boat::StopRotateBoatRight, this));

		//Shoot boat
		action_ShootBoat->OnKeyTriggered.push_back(BindAction(&Boat::StartShootBullet, this));
		action_ShootBoat->OnKeyReleased.push_back(BindAction(&Boat::StopShootBullet, this));

		initialSprite.setColor(sf::Color::Cyan);
	}
}

void Boat::Init()
{
	timer.restart();
}

void Boat::Update()
{
	Player::Update();

	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		if (bIsBoatAccelerating)
			AccelerateBoat();
		if (bIsBoatRotatingLeft)
			RotateBoatLeft();
		if (bIsBoatRotatingRight)
			RotateBoatRight();
		if (bBoatIsShooting)
			BoatShootBullet();
	}
}

void Boat::SetIsAccelerating(bool bIsAcelerating)
{
	bIsBoatAccelerating = bIsAcelerating;
}

void Boat::SetIsRotatingLeft(bool bRotatingLeft)
{
	bIsBoatRotatingLeft = bRotatingLeft;
}

void Boat::SetIsRotatingRight(bool bRotatingRight)
{
	bIsBoatRotatingRight = bRotatingRight;
}

void Boat::SetIsShooting(bool bIsShooting)
{
	bBoatIsShooting = bIsShooting;
}

void Boat::StartAccelerateBoat()
{
	bIsBoatAccelerating = true;
	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		AddLocalNetworkDataToSend(key_AccelerateBoatID, bIsBoatAccelerating);
	}

}

void Boat::AccelerateBoat()
{
	NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();

	//std::cout << "Accelerating Boat\n";
	//sf::Vector2f position = ApplicationHelper::Normalize(position);
	sf::Vector2f position = initialSprite.getPosition();

	speed = 750 * ApplicationHelper::GetDeltaTime();

	position += forwardVector * speed;


	//position.y -= 100 * ApplicationHelper::GetDeltaTime();
	SetPosition(position);

#if _DEBUG
	//timesAccelerated++;
	//std::cout << "Boat " << playerID << ": " << timesAccelerated << "times accelerated" << std::endl;
#endif
	Json::Value boatPosition;
	boatPosition.append(position.x);
	boatPosition.append(position.y);

	//Send the data to the clients 
	AddLocalNetworkDataToSend(key_boatPosition, boatPosition);
}

void Boat::StopAccelerateBoat()
{
	bIsBoatAccelerating = false;
	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		AddLocalNetworkDataToSend(key_AccelerateBoatID, bIsBoatAccelerating);
	}
}

void Boat::DecelerateBoat()
{
	std::cout << "Accelerating Boat\n";
	//sf::Vector2f position = ApplicationHelper::Normalize(position);
	sf::Vector2f position = initialSprite.getPosition();
	position.y += 100 * ApplicationHelper::GetDeltaTime();
	initialSprite.setPosition(position);
}

void Boat::StartRotateBoatLeft()
{
	bIsBoatRotatingLeft = true;
	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		// Function ID + PlayerID
		AddLocalNetworkDataToSend(key_RotateBoatLeftID, bIsBoatRotatingLeft);
	}
}

void Boat::RotateBoatLeft()
{
	NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();

	std::cout << "Rotating boat left\n";
	float tickRotation = -angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
	float previousRotation = GetRotation();
	float newAngleRotation = previousRotation + tickRotation;
	SetRotation(newAngleRotation);
	AddLocalNetworkDataToSend(key_boatAngle, GetRotation());
}

void Boat::StopRotateBoatLeft()
{
	bIsBoatRotatingLeft = false;
	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		// Function ID + PlayerID
		AddLocalNetworkDataToSend(key_RotateBoatLeftID, bIsBoatRotatingLeft);
	}
}

void Boat::StartShootBullet()
{
	bBoatIsShooting = true;
	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		AddLocalNetworkDataToSend(key_ShootBoatID, bBoatIsShooting);
	}
		
}

void Boat::BoatShootBullet()
{
	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		sf::Time elapsedTime = timer.getElapsedTime();
		if (elapsedTime.asSeconds() >= shootingCD) {
			std::cout << "Spawn Bullet\n";

			bullets[bulletTracker]->SetGameObjectTransform(GetShootingLocation(), GetRotation(), bullets[bulletTracker]->GetScale());
			bullets[bulletTracker]->bTickEnabled = true; 
			bullets[bulletTracker]->ShowGameObject();
			Sound* sound = SoundManager::Get()->GetSound("Sound/Boat/blaster.wav");

			if (sound)
			{
				sound->SetVolume(20.f);
				sound->PlaySound();
			}
			bulletTracker++; 
			
			if (bulletTracker >= MaxBulletsPerBoatOnScreen)
				bulletTracker = 0; 

			timer.restart();
		}
	}
}

void Boat::StopShootBullet()
{
	bBoatIsShooting = false;
	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		AddLocalNetworkDataToSend(key_ShootBoatID, bBoatIsShooting);
	}
}

void Boat::StartRotateBoatRight()
{
	bIsBoatRotatingRight = true;
	// Function ID + PlayerID

	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		AddLocalNetworkDataToSend(key_RotateBoatRightID, bIsBoatRotatingRight);
	}
}

void Boat::RotateBoatRight()
{
	NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();

		std::cout << "Rotating boat right\n";
		float tickRotation = angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
		float previousRotation = GetRotation();
		float newAngleRotation = previousRotation + tickRotation;
		SetRotation(newAngleRotation);
		AddLocalNetworkDataToSend(key_boatAngle, GetRotation());
}

void Boat::StopRotateBoatRight()
{
	bIsBoatRotatingRight = false;

	if (!AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		// Function ID + PlayerID
		AddLocalNetworkDataToSend(key_RotateBoatRightID, bIsBoatRotatingRight);
	}
}

sf::Vector2f Boat::GetShootingLocation()
{
	return GetPosition() + rightVector * 12.f  + forwardVector * 50.f;
}
