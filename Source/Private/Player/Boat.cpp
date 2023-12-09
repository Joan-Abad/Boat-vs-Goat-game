#include "Player/Boat.h"
#include <iostream>
#include "ApplicationHelper.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Input/InputManager.h"

const char* Boat::key_AccelerateBoatID = "AccelerateBoatID";
const char* Boat::key_RotateBoatLeftID = "RotateBoatLeftID";
const char* Boat::key_RotateBoatRightID = "RotateBoatRightID";
const char* Boat::key_boatPosition = "boatPosition";
const char* Boat::key_boatAngle = "boatAngle";
const char* Boat::key_boatID= "boatID";


Boat::Boat(sf::Window& window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo) : Player(window, PlayerPlayable, playerInitialInfo), angleBoatSpeedEachSecond(360.f), 
bIsBoatAccelerating (false), bIsBoatRotatingLeft(false), bIsBoatRotatingRight(false)
{
	if (PlayerPlayable)
	{
		InputManager* im = InputManager::GetInputManager();
		action_W = im->GetInputAction(sf::Keyboard::W);
		action_RotateLeft = im->GetInputAction(sf::Keyboard::Key::Q);
		action_RotateRight = im->GetInputAction(sf::Keyboard::Key::E);

		action_W->OnKeyTriggered.push_back(BindAction(&Boat::StartAccelerateBoat, this));
		//action_W->OnKeyOnGoing = BindAction(&Boat::AccelerateBoat, this);
		action_W->OnKeyReleased.push_back(BindAction(&Boat::StopAccelerateBoat, this));

		action_RotateLeft->OnKeyTriggered.push_back(BindAction(&Boat::StartRotateBoatLeft, this));
		//action_W->OnKeyOnGoing = BindAction(&Boat::AccelerateBoat, this);
		action_RotateLeft->OnKeyReleased.push_back(BindAction(&Boat::StopRotateBoatLeft, this));

		action_RotateRight->OnKeyTriggered.push_back(BindAction(&Boat::StartRotateBoatRight, this));
		//action_W->OnKeyOnGoing = BindAction(&Boat::AccelerateBoat, this);
		action_RotateRight->OnKeyReleased.push_back(BindAction(&Boat::StopRotateBoatRight, this));

		//action_RotateLeft->OnKeyOnGoing = BindAction(&Boat::RotateBoatLeft, this);
		//action_RotateRight->OnKeyOnGoing = BindAction(&Boat::RotateBoatRight, this);
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

	if (bIsBoatAccelerating)
		AccelerateBoat();
	if (bIsBoatRotatingLeft)
		RotateBoatLeft();
	if (bIsBoatRotatingRight)
		RotateBoatRight();
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
	sf::Vector2f position = playerSprite.getPosition();

	speed = 500 * ApplicationHelper::GetDeltaTime();

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
	sf::Vector2f position = playerSprite.getPosition();
	position.y += 100 * ApplicationHelper::GetDeltaTime();
	playerSprite.setPosition(position);
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
