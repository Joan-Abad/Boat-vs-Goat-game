#include "Player/Boat.h"
#include <iostream>
#include "ApplicationHelper.h"
#include "Managers/Networking/NetworkingManager.h"

const char* Boat::key_AccelerateBoatID = "AccelerateBoatID";
const char* Boat::key_RotateBoatLeftID = "RotateBoatLeftID";
const char* Boat::key_RotateBoatRightID = "RotateBoatRightID";

Boat::Boat(sf::Window& window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo) : Player(window, PlayerPlayable, playerInitialInfo), angleBoatSpeedEachSecond(360.f)
{
	if (PlayerPlayable)
	{
		action_W = InputAction::CreateInputAction(GetPlayerActions(), sf::Keyboard::Key::W);
		action_RotateLeft = InputAction::CreateInputAction(GetPlayerActions(), sf::Keyboard::Key::Q);
		action_RotateRight = InputAction::CreateInputAction(GetPlayerActions(), sf::Keyboard::Key::E);

		action_W->OnKeyOnGoing = BindAction(&Boat::AccelerateBoat, this);
		action_RotateLeft->OnKeyOnGoing = BindAction(&Boat::RotateBoatLeft, this);
		action_RotateRight->OnKeyOnGoing = BindAction(&Boat::RotateBoatRight, this);
	}
}

void Boat::HandlePlayerInput()
{
	Player::HandlePlayerInput();
}

void Boat::Update()
{
	Player::Update();

}

void Boat::AccelerateBoat()
{
	if (GetNetworkingManager()->GetIsServer())
	{
		std::cout << "Accelerating Boat\n";
		//sf::Vector2f position = ApplicationHelper::Normalize(position);
		sf::Vector2f position = playerSprite.getPosition();

		speed = 500 * ApplicationHelper::GetDeltaTime();

		position += forwardVector * speed;

		//position.y -= 100 * ApplicationHelper::GetDeltaTime();
		SetPosition(position);

#if _DEBUG
		timesAccelerated++;
		std::cout << "Times accelerated: " << timesAccelerated << std::endl;
#endif
	}
	else
	{
		NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();
		// Function ID + PlayerID
		netManager.AddNetworkDataToSend(key_AccelerateBoatID, netManager.GetPlayerID());
	}
	
}

void Boat::DecelerateBoat()
{
	std::cout << "Accelerating Boat\n";
	//sf::Vector2f position = ApplicationHelper::Normalize(position);
	sf::Vector2f position = playerSprite.getPosition();
	position.y += 100 * ApplicationHelper::GetDeltaTime();
	playerSprite.setPosition(position);
}

void Boat::RotateBoatLeft()
{
	if (GetNetworkingManager()->GetIsServer())
	{
		std::cout << "Rotating boat left\n";
		float tickRotation = -angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
		float previousRotation = GetRotation();
		float newAngleRotation = previousRotation + tickRotation;
		SetRotation(newAngleRotation);
	}
	else
	{
		NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();
		// Function ID + PlayerID
		netManager.AddNetworkDataToSend(key_RotateBoatLeftID, netManager.GetPlayerID());
	}
}

void Boat::RotateBoatRight()
{
	if (GetNetworkingManager()->GetIsServer())
	{
		std::cout << "Rotating boat right\n";
		float tickRotation = angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
		float previousRotation = GetRotation();
		float newAngleRotation = previousRotation + tickRotation;
		SetRotation(newAngleRotation);
	}
	else
	{
		NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();
		// Function ID + PlayerID
		netManager.AddNetworkDataToSend(key_RotateBoatRightID, netManager.GetPlayerID());
	}
}
