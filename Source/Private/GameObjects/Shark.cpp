#include "GameObjects/Shark.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "ApplicationHelper.h"
#include "Managers/SoundManager.h"

sf::Texture* Shark::sharkTexture = nullptr;
//Spawn shark sound
const char* Shark::key_spawnSharkNoise = "spSS";

Shark::Shark(sf::Vector2f initLocation, float radius) : GameObject(), timeToReachLocation(4.f), alphaTracker(0.f)
{
	this->initLocation = initLocation;
	lastLocation = initLocation; 
	this->radius = radius; 
	objectCollision = CollisionChannels::Bullet; 


	SetPosition(initLocation);
	initialSprite.setTexture(*sharkTexture);
	
	SetNewLocationToMove();
	initialSprite.setOrigin(initialSprite.getLocalBounds().width / 2, initialSprite.getLocalBounds().height / 2);
	
	float angle = FindRotationDirection(initLocation);
	SetRotation(angle);
	SetScale(sf::Vector2f(0.6f, 0.6f));
}

void Shark::Update(float deltaTime)
{
	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		alphaTracker += deltaTime / timeToReachLocation;
		sf::Vector2f newPosition = ApplicationHelper::Lerp(lastLocation, destinationLocation, alphaTracker);
		SetPosition(newPosition);
		Json::Value pos;
		pos.append(newPosition.x);
		pos.append(newPosition.y);
		AddLocalNetworkDataToSend(key_gameObjectPosition, pos);
		if (alphaTracker >= 1)
		{
			alphaTracker = 0; 
			lastLocation = destinationLocation;
			SetNewLocationToMove();	
			float angle = FindRotationDirection(lastLocation);
			SetRotation(angle);
			AddLocalNetworkDataToSend(key_gameObjectRot, angle);
		}
	}
}

void Shark::SetNewLocationToMove()
{
	sf::Vector2f sharkDirection = sf::Vector2f(1.f, 0.f);
	float directionRotaiton = ApplicationHelper::GetRandomValue(0.f, 360.f);
	sf::Vector2f directedDistance = ApplicationHelper::rotateVector(sharkDirection, directionRotaiton) * radius;

	this->destinationLocation = this->initLocation + directedDistance;

}

float Shark::FindRotationDirection(sf::Vector2f startingLocation)
{
	sf::Vector2f posDiff = destinationLocation - startingLocation;
	posDiff.y *= -1;

	float angle = ApplicationHelper::AngleBetweenTwoVectors(sf::Vector2f(0, 1.0f), posDiff);

	if (posDiff.x < 0)
		angle *= -1;

	return angle;
}

void Shark::UpdateClientNetData(const Json::Value& root)
{
	if (root.isMember(key_gameObjectPosition))
	{
		const Json::Value& boatPositionArray = root[key_gameObjectPosition];
		sf::Vector2f boatPosition = sf::Vector2f(boatPositionArray[0].asFloat(), boatPositionArray[1].asFloat());
		SetPosition(boatPosition);
	}
	if (root.isMember(key_gameObjectRot))
	{
		float rot = root[key_gameObjectRot].asFloat();
		SetRotation(rot);
	}
	if (root.isMember(key_spawnSharkNoise))
	{
		SoundManager::Get()->GetSound("Sound/SharkBite.wav")->PlaySound();
	}
}
