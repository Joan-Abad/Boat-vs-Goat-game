#include "Player/Player.h"
#include <iostream>
#include <SFML/Network.hpp>
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/Networking/NetworkingManagerServer.h"
#include "Managers/Networking/NetworkingManagerClient.h"
#include "ApplicationHelper.h"
#include <json.h>
#include <vector>

sf::Vector2f calculateForwardVectorDegrees(float angleDegrees) {
	float angleRadians = angleDegrees * 3.14159265358979323846f / 180.f; // Convert degrees to radians
	return sf::Vector2f(std::cos(angleRadians), std::sin(angleRadians));
}

sf::Vector2f normalize(const sf::Vector2f& vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);

	if (length != 0) {
		return sf::Vector2f(vector.x / length, vector.y / length);
	}
	else {
		// Handle division by zero or zero-length vectors
		return sf::Vector2f(0.f, 0.f);
	}
}

Player::Player(sf::Window& window, bool isLocallyController, PlayerInitialInfo InitialInfo)
{
	this->window = &window;

	//Player Input Actions
	this->bIsLocallyControlled = isLocallyController;

	forwardVector = sf::Vector2f(0.f, 1.0f);

	if (!playerTexture.loadFromFile(InitialInfo.playerTexturePath))
		std::cerr << "BackgroundPath error\n";

	playerSprite.setTexture(playerTexture);

	// Set the origin to the center of the sprite
	playerSprite.setOrigin(playerSprite.getLocalBounds().width / 2, playerSprite.getLocalBounds().height / 2);

	SetRotation(InitialInfo.angle);
	SetPosition(InitialInfo.playerPosition);

	root.clear();

}

void Player::HandlePlayerInput()
{
	if (bIsLocallyControlled)
	{
		sf::Event inputEvent;
		while (window->pollEvent(inputEvent))
		{
			if (inputEvent.type == sf::Event::Closed)
				window->close();
		}
	}
}

void Player::CheckKeyPressed(InputAction& inputAction)
{
	bool bIsSpacePressedThisFrame = sf::Keyboard::isKeyPressed(inputAction.GetKey());

	if (bIsSpacePressedThisFrame)
	{
		//On Trigger
		if (!inputAction.GetIsKeyPressed())
		{
			inputAction.SetIsKeyPressed(true);
			if (inputAction.OnKeyTriggered)
				inputAction.OnKeyTriggered(); 
		}
		//On Going
		if(inputAction.OnKeyOnGoing)
			inputAction.OnKeyOnGoing();
	}
	else
	{
		//On Released
		if (inputAction.GetIsKeyPressed())
		{
			if (inputAction.OnKeyReleased)
				inputAction.OnKeyReleased();
		}
		inputAction.SetIsKeyPressed(false);
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(playerSprite);
}

float Player::GetRotation()
{
	return playerSprite.getRotation();
}

void Player::Update()
{
}

void Player::SetPosition(sf::Vector2f newPosition)
{
	playerSprite.setPosition(newPosition);
	Json::Value floatArray; 
	floatArray.append(newPosition.x);
	floatArray.append(newPosition.y);

	root["playerPosition"] = floatArray;
}

void Player::SetRotation(float angle)
{
	forwardVector = ApplicationHelper::rotateVector(sf::Vector2f(0.0f, 1.0f), angle);
	playerSprite.setRotation(angle);
	root["playerAngle"] = angle;
}

void Player::UpdaetPlayerInfo(const std::string& NetworkData)
{
	if (!bIsLocallyControlled)
	{
		//Extract the data and update player info
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(NetworkData, root);
		Json::Value coordinatesArray = root["playerPosition"];
		float a = coordinatesArray[0].asFloat();
		float b = coordinatesArray[1].asFloat();
		float angle = root["playerAngle"].asFloat();
		SetPosition(sf::Vector2f(a,b));
		SetRotation(angle);
	}
}

InputAction::InputAction()
{

}

InputAction::InputAction(sf::Keyboard::Key key)
{
	this->key = key;
}

void InputAction::SetKey(sf::Keyboard::Key key)
{
	this->key = key;
}

void InputAction::SetIsKeyPressed(bool keyPressed)
{
	bIsKeyPressed = keyPressed;
}
