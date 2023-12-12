#include "GameObjects/Players/Player.h"
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

Player::Player(bool isLocallyController, GameObjectInitialInfo info) : GameObject(GameObjectInitialInfo(info.playerPosition, info.angle))
{

	//Player Input Actions
	this->bIsLocallyControlled = isLocallyController;

	TextureManager& textureManager = TextureManager::GetTextureManager();

	//initialSprite.setTexture(*textureManager.GetTexture(info.GameObjectTexturePath));

	// Set the origin to the center of the sprite

	SetRotation(info.angle);
	SetPosition(info.playerPosition);

	playerID = playerTrackerID;
	playerTrackerID++;

	gameObjectNetData.clear();

}
Player::~Player()
{

}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(initialSprite);
}

void Player::Update()
{
}

NetworkingManager* Player::GetNetworkingManager()
{
	return AppManager::GetAppManager()->GetNetworkManager();
}

