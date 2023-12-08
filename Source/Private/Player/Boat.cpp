#include "Player/Boat.h"
#include <iostream>
#include "ApplicationHelper.h"
#include "Managers/Networking/NetworkingManager.h"

const char* Boat::key_AccelerateBoatID = "AccelerateBoatID";
const char* Boat::key_RotateBoatLeftID = "RotateBoatLeftID";
const char* Boat::key_RotateBoatRightID = "RotateBoatRightID";
const char* Boat::key_boatPosition = "boatPosition";
const char* Boat::key_boatAngle = "boatAngle";
const char* Boat::key_boatID= "boatID";


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
		playerSprite.setColor(sf::Color::Red);
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
	NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();

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
		std::cout << "Boat " << playerID << ": " << timesAccelerated << "times accelerated" << std::endl;
#endif
		Json::Value boatPosition;
		boatPosition.append(position.x);
		boatPosition.append(position.y);

		//Send the data to the clients 
		AddLocalNetworkDataToSend(key_boatPosition, boatPosition);

	}
	else
	{	
		// Function ID + PlayerID
		AddLocalNetworkDataToSend(key_AccelerateBoatID, playerID);
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
	NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();

	if (GetNetworkingManager()->GetIsServer())
	{
		std::cout << "Rotating boat left\n";
		float tickRotation = -angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
		float previousRotation = GetRotation();
		float newAngleRotation = previousRotation + tickRotation;
		SetRotation(newAngleRotation);
		AddLocalNetworkDataToSend(key_boatAngle, GetRotation());
	}
	else
	{
		// Function ID + PlayerID
		AddLocalNetworkDataToSend(key_RotateBoatLeftID, playerID);
	}
}

void Boat::RotateBoatRight()
{
	NetworkingManager& netManager = *AppManager::GetAppManager()->GetNetworkManager();

	if (GetNetworkingManager()->GetIsServer())
	{
		std::cout << "Rotating boat right\n";
		float tickRotation = angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
		float previousRotation = GetRotation();
		float newAngleRotation = previousRotation + tickRotation;
		SetRotation(newAngleRotation);
		AddLocalNetworkDataToSend(key_boatAngle, GetRotation());
	}
	else
	{
		// Function ID + PlayerID
		AddLocalNetworkDataToSend(key_RotateBoatRightID, playerID);
	}
}
