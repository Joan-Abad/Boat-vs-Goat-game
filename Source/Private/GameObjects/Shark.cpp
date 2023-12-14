#include "GameObjects/Shark.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "ApplicationHelper.h"

sf::Texture* Shark::sharkTexture = nullptr;

Shark::Shark(sf::Vector2f initLocation, float radius) : GameObject(), timeToReachLocation(4.f), alphaTracker(0.f)
{
	this->initLocation = initLocation;
	lastLocation = initLocation; 
	this->radius = radius; 
	objectCollision = CollisionChannels::Bullet; 


	SetPosition(initLocation);
	initialSprite.setTexture(*sharkTexture);
	SetMoveToLocation();
	initialSprite.setOrigin(initialSprite.getLocalBounds().width / 2, initialSprite.getLocalBounds().height / 2);
	float angle = ApplicationHelper::AngleBetweenTwoVectors(GetForwardVector(), destinationLocation - initLocation);
	SetRotation(angle);
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
			SetMoveToLocation();
			sf::Vector2f vec = destinationLocation - lastLocation;
			float angle = ApplicationHelper::AngleBetweenTwoVectors(GetForwardVector(), vec);
			SetRotation(angle);
			std::cout << "SharkAngle: " << angle << std::endl; 
			AddLocalNetworkDataToSend(key_gameObjectRot, angle);
		}
	}
}

void Shark::SetMoveToLocation()
{
	sf::Vector2f sharkDirection = sf::Vector2f(1.f, 0.f);
	float directionRotaiton = ApplicationHelper::GetRandomValue(0.f, 360.f);
	sf::Vector2f directedDistance = ApplicationHelper::rotateVector(sharkDirection, directionRotaiton) * radius;

	this->destinationLocation = this->initLocation + directedDistance;

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
}
