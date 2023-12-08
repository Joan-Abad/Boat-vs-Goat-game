#include "Player/Player.h"
#include <iostream>
#include <SFML/Network.hpp>
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/Networking/NetworkingManagerServer.h"
#include "Managers/Networking/NetworkingManagerClient.h"
#include "ApplicationHelper.h"
#include <json.h>
#include <vector>
#include "Managers/TextureManager.h"
#include "Managers/AppManager.h"
#include "Input/InputManager.h"

std::vector<sf::Keyboard::Key> InputAction::keysPressed;
int Player::playerTrackerID = 0;

Player::Player(sf::Window& window, bool isLocallyController, PlayerInitialInfo InitialInfo)
{
	this->window = &window;

	//Player Input Actions
	this->bIsLocallyControlled = isLocallyController;

	forwardVector = sf::Vector2f(0.f, 1.0f);


	TextureManager& textureManager = TextureManager::GetTextureManager();

	playerSprite.setTexture(*textureManager.GetTexture(InitialInfo.playerTexturePath));

	// Set the origin to the center of the sprite
	playerSprite.setOrigin(playerSprite.getLocalBounds().width / 2, playerSprite.getLocalBounds().height / 2);

	SetRotation(InitialInfo.angle);
	SetPosition(InitialInfo.playerPosition);

	playerID = playerTrackerID;
	playerTrackerID++;

	localRootData.clear();

}

Player::~Player()
{
	for (auto& playerAction : playerActions)
		delete playerAction; 
}

void Player::HandlePlayerInput()
{
	if (bIsLocallyControlled)
	{
		for (auto& keyPressedEvent : InputManager::GetInputManager()->GetKeysPressed())
		{
			for (auto& playerAction : playerActions)
			{
				//Will handle for now only one action for keyboard key
				if (playerAction->CheckIfTriggerAction(keyPressedEvent))
					break;
			}
		}
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
	HandlePlayerInput();
}

void Player::EndUpdate()
{
	
	if (!localRootData.empty() && (!GetNetworkingManager()->GetIsServer() && bIsLocallyControlled || GetNetworkingManager()->GetIsServer()))
	{
		AddLocalNetworkDataToSend(NetworkingManager::key_PlayerID, playerID);
		AddLocalNetworkDataToRootData();
	}
}

void Player::SetPosition(sf::Vector2f newPosition)
{
	playerSprite.setPosition(newPosition);
	Json::Value floatArray; 
	floatArray.append(newPosition.x);
	floatArray.append(newPosition.y);

	//root["playerPosition"] = floatArray;
}

void Player::SetRotation(float angle)
{
	forwardVector = ApplicationHelper::rotateVector(sf::Vector2f(0.0f, -1.0f), angle);
	playerSprite.setRotation(angle);
	//root["playerAngle"] = angle;
}

NetworkingManager* Player::GetNetworkingManager()
{
	return AppManager::GetAppManager()->GetNetworkManager();
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

void Player::AddLocalNetworkDataToRootData()
{
	if (!localRootData.empty())
	{
		Json::Value& value = AppManager::GetAppManager()->GetNetworkManager()->GetRootData();
		value.copy(localRootData);
		localRootData.clear();
	}
}

InputAction::InputAction()
{
}

InputAction::InputAction(sf::Keyboard::Key key)
{
	this->key = key;
}

InputAction* InputAction::CreateInputAction(std::vector<InputAction*> &playerActions, sf::Keyboard::Key key)
{
	// TODO: insert return statement here

	InputAction* newAction = new InputAction(key);
	if (newAction)
		playerActions.push_back(newAction);
	else
		std::cerr << "Could not create the input action\n";

	return newAction;
}

bool InputAction::CheckIfTriggerAction(sf::Keyboard::Key inComingKey)
{
	bool bIsSpacePressedThisFrame = inComingKey == key;

	if (bIsSpacePressedThisFrame)
	{
		//On Trigger
		if (!GetIsKeyPressed())
		{
			SetIsKeyPressed(true);
			if (OnKeyTriggered)
				OnKeyTriggered();
		}
		//On Going
		if (OnKeyOnGoing)
			OnKeyOnGoing();
	}
	else
	{
		//On Released
		if (GetIsKeyPressed())
		{
			if (OnKeyReleased)
				OnKeyReleased();
		}
		SetIsKeyPressed(false);
	}

	return bIsSpacePressedThisFrame;
}

void InputAction::SetKey(sf::Keyboard::Key key)
{
	this->key = key;
}

void InputAction::SetIsKeyPressed(bool keyPressed)
{
	bIsKeyPressed = keyPressed;
}
