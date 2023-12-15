#include "GameObjects/Cannon.h"
#include "Managers/TextureManager.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "ApplicationHelper.h"
#include "GameObjects/Bullet.h"

const char* Cannon::key_SpawnMissile = "spMI";
const char* Cannon::key_SpawnPos = "spPo";
const char* Cannon::key_SpawnRot = "spRo";

float Cannon::minCannonBallSpeed = 200.f;
float Cannon::maxCannonBallSpeed = 250.f;
Sound* Cannon::cannonSound = nullptr; 

Cannon::Cannon(GameObjectInitialInfo initialInfo) : tickAcceleration(sf::Vector2f(0.0f, 0.0f))
{
	initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture("Art/cannon.png"));
	objectCollision = CollisionChannels::NoCollision;
	CollisionsToRespond.push_back(CollisionChannels::Player);
	CollisionsToRespond.push_back(CollisionChannels::WorldStatic);
	bTickEnabled = false; 
	bReplicates = true; 
	initialSprite.setScale(0.2f, 0.2f);
	initialSprite.setOrigin(initialSprite.getLocalBounds().width / 2, initialSprite.getLocalBounds().height / 2);

}

void Cannon::InitMissile(sf::Vector2f position, float angle)
{
	ShowGameObject();
	bTickEnabled = true; 
	SetPosition(position);
	SetRotation(angle);
	objectCollision = CollisionChannels::Bullet;
}

void Cannon::Update(float deltaTime)
{
	//if (AppManager::GetNetworkManager()->GetIsServer())
	//{
	//	//Accelerate boat bullet on forward vector direction
	//	sf::Vector2f bulletPosition = GetPosition() + GetForwardVector() * cannonBallSpeed * deltaTime;
	//	SetPosition(bulletPosition);

	//	Json::Value pos;
	//	pos.append(bulletPosition.x);
	//	pos.append(bulletPosition.y);

	//	AddLocalNetworkDataToSend(key_gameObjectPosition, pos);
	//}
}

void Cannon::ThrowCannonBall(Bullet& bullet)
{
	sf::Vector2f spawnPoint = GetPosition() + GetRightVector() * 12.5f;
	bullet.ShowGameObject();
	bullet.SetPosition(spawnPoint);
	bullet.SetRotation(GetRotation());
	bullet.objectCollision = CollisionChannels::Bullet;
	bullet.bTickEnabled = true; 
	
	float bulletSpeed = ApplicationHelper::GetRandomValue<float>(minCannonBallSpeed, maxCannonBallSpeed);
	bullet.SetBulletSpeed(bulletSpeed);
	cannonSound->PlaySound();
	cannonSound->SetVolume(50.f);
	//Replication code
	bullet.AddLocalNetworkDataToSend(key_gameObjectHide, false);
	Json::Value pos;
	pos.append(spawnPoint.x);
	pos.append(spawnPoint.y);

	Json::StreamWriterBuilder writerBuilder;
	std::string msgToSend = Json::writeString(writerBuilder, pos);

	std::cout << "BoatPosition x: " << spawnPoint.x << " - y: " << spawnPoint.y << std::endl;
	bullet.AddLocalNetworkDataToSend(key_SpawnPos, pos);
	bullet.AddLocalNetworkDataToSend(key_SpawnRot, GetRotation());
}


void Cannon::UpdateClientNetData(const Json::Value& root)
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
			const Json::Value& missilePositionArray = root[Cannon::key_SpawnPos];
			missilePosition  = sf::Vector2f(missilePositionArray[0].asFloat(), missilePositionArray[1].asFloat());
		}
		if (root.isMember(key_SpawnRot))
		{
			angle = root[Cannon::key_SpawnRot].asFloat();
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

void Cannon::OnCollisionEnter(GameObject* otherGO)
{
	if (otherGO)
	{
		objectCollision = CollisionChannels::NoCollision;
		HideGameObject();

	}
}
