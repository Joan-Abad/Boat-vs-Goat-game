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

Player::Player(GameObjectInitialInfo info, bool isLocallyController) : GameObject(info)
{
	objectCollision = CollisionChannels::Player;
	CollisionsToRespond.push_back(CollisionChannels::Bullet);
	CollisionsToRespond.push_back(CollisionChannels::WorldStatic);

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

void Player::Update()
{
}

NetworkingManager* Player::GetNetworkingManager()
{
	return AppManager::GetAppManager()->GetNetworkManager();
}

