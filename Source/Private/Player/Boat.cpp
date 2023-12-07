#include "Player/Boat.h"
#include <iostream>
#include "ApplicationHelper.h"

Boat::Boat(sf::Window& window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo) : Player(window, PlayerPlayable, playerInitialInfo)
{

}

void Boat::HandlePlayerInput()
{
	Player::HandlePlayerInput();
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
	float tickRotation = -angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
	float previousRotation = GetRotation();
	float newAngleRotation = previousRotation + tickRotation;
	SetRotation(newAngleRotation);
}

void Boat::RotateBoatRight()
{
	float tickRotation = angleBoatSpeedEachSecond * ApplicationHelper::GetDeltaTime();
	float previousRotation = GetRotation();
	float newAngleRotation = previousRotation + tickRotation;
	SetRotation(newAngleRotation);
}
