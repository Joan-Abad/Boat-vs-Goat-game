#include "GameObjects/Missile.h"
#include "Managers/TextureManager.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "ApplicationHelper.h"

const char* Missile::key_SpawnMissile = "spMI";
const char* Missile::key_SpawnPos = "spPo";
const char* Missile::key_SpawnRot = "spRo";


Missile::Missile(GameObjectInitialInfo initialInfo) : tickAcceleration(sf::Vector2f(0.0f, 0.0f)), missileSpeed(300.f)
{
	initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture("Art/missile.png"));
	objectCollision = CollisionChannels::NoCollision;
	CollisionsToRespond.push_back(CollisionChannels::Player);
	CollisionsToRespond.push_back(CollisionChannels::WorldStatic);
	bTickEnabled = false; 
	bReplicates = true; 
	initialSprite.setScale(0.2f, 0.2f);
}

void Missile::InitMissile(sf::Vector2f position, float angle)
{
	ShowGameObject();
	bTickEnabled = true; 
	SetPosition(position);
	SetRotation(angle);
	objectCollision = CollisionChannels::Bullet;
}

void Missile::Update()
{
	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		//Accelerate boat bullet on forward vector direction
		sf::Vector2f bulletPosition = GetPosition() + GetForwardVector() * missileSpeed * ApplicationHelper::GetDeltaTime();
		SetPosition(bulletPosition);

		Json::Value pos;
		pos.append(bulletPosition.x);
		pos.append(bulletPosition.y);

		AddLocalNetworkDataToSend(key_gameObjectPosition, pos);
	}
}

void Missile::UpdateClientNetData(const Json::Value& root)
{
	if (root.isMember(key_gameObjectHide))
	{
		HideGameObject();
		objectCollision = CollisionChannels::NoCollision;
	}

	if (root.isMember(key_SpawnMissile))
	{
		sf::Vector2f missilePosition = {0,0};
		float angle = 0; 
		if (root.isMember(key_SpawnPos))
		{
			const Json::Value& missilePositionArray = root[Missile::key_SpawnPos];
			missilePosition  = sf::Vector2f(missilePositionArray[0].asFloat(), missilePositionArray[1].asFloat());
		}
		if (root.isMember(key_SpawnRot))
		{
			angle = root[Missile::key_SpawnRot].asFloat();
		}
		
		InitMissile(missilePosition, angle);
	}

	//Set position
	if (root.isMember(key_gameObjectPosition))
	{
		const Json::Value& missilePositionArray = root[GameObject::key_gameObjectPosition];
		sf::Vector2f missilePosition = sf::Vector2f(missilePositionArray[0].asFloat(), missilePositionArray[1].asFloat());
		SetPosition(missilePosition);
	}
}

void Missile::OnCollisionEnter(GameObject* otherGO)
{
	
}
