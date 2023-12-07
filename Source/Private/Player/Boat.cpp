#include "Player/Boat.h"
#include <iostream>
#include "ApplicationHelper.h"

Boat::Boat(sf::Window& window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo) : Player(window, PlayerPlayable, playerInitialInfo), angleBoatSpeedEachSecond(360.f)
{
	if (PlayerPlayable)
	{
		action_W.SetKey(sf::Keyboard::Key::W);
		action_RotateLeft.SetKey(sf::Keyboard::Key::Q);
		action_RotateRight.SetKey(sf::Keyboard::Key::E);

		action_W.OnKeyOnGoing = BindAction(&Boat::AccelerateBoat, this);
		action_RotateLeft.OnKeyOnGoing = BindAction(&Boat::RotateBoatLeft, this);
		action_RotateRight.OnKeyOnGoing = BindAction(&Boat::RotateBoatRight, this);
	}
}

void Boat::HandlePlayerInput()
{
	Player::HandlePlayerInput();

	CheckKeyPressed(action_W);
	CheckKeyPressed(action_RotateLeft);
	CheckKeyPressed(action_RotateRight);
}

void Boat::Update()
{
	Player::Update();

}

void Boat::AccelerateBoat()
{
	std::cout << "Accelerating Boat\n";
	//sf::Vector2f position = ApplicationHelper::Normalize(position);
	sf::Vector2f position = playerSprite.getPosition();

	speed = 500 * ApplicationHelper::GetDeltaTime();

	position += forwardVector * speed;

	//position.y -= 100 * ApplicationHelper::GetDeltaTime();
	SetPosition(position);
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
	std::cout << "Rotating boat left\n";
	float tickRotation = -angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
	float previousRotation = GetRotation();
	float newAngleRotation = previousRotation + tickRotation;
	SetRotation(newAngleRotation);
}

void Boat::RotateBoatRight()
{
	std::cout << "Rotating boat right\n";
	float tickRotation = angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
	float previousRotation = GetRotation();
	float newAngleRotation = previousRotation + tickRotation;
	SetRotation(newAngleRotation);
}
