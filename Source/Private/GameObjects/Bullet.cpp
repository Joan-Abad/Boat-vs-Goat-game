#include "GameObjects/Bullet.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/TextureManager.h"
#include "ApplicationHelper.h"
#include "Managers/SoundManager.h"
#include "CollisionChannel/CollisionChannels.h"

Bullet::Bullet() : GameObject()
{
	bTickEnabled = false;
	CollisionsToRespond.push_back(CollisionChannels::Player);
	CollisionsToRespond.push_back(CollisionChannels::WorldStatic);
}

Bullet::Bullet(GameObjectInitialInfo initialInfo) : GameObject(initialInfo), bulletSpeed(1200.f)
{
	bIgnoreOwner = false; 
	bTickEnabled = false;
	CollisionsToRespond.push_back(CollisionChannels::Player);
	CollisionsToRespond.push_back(CollisionChannels::WorldStatic);
	objectCollision = CollisionChannels::NoCollision;
	initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture("Art/bullet.png"));
	initialSprite.setScale(0.035f, 0.035f);
}

void Bullet::Update(float deltaTime)
{
	if (AppManager::GetNetworkManager()->GetIsServer())
	{
		//Stop updating the object if it is out of screen
		if (GameObjectOutOfScreen())
		{
			bTickEnabled = false;
			HideGameObject();
			AddLocalNetworkDataToSend(key_gameObjectHide, true);
		}
		else
		{
			//Accelerate boat bullet on forward vector direction
			sf::Vector2f bulletPosition = GetPosition() + GetForwardVector() * bulletSpeed * deltaTime;
			SetPosition(bulletPosition);

			Json::Value pos; 
			pos.append(bulletPosition.x); 
			pos.append(bulletPosition.y);

			AddLocalNetworkDataToSend(key_gameObjectPosition, pos);
			
		}
	}
}

void Bullet::UpdateClientNetData(const Json::Value& root)
{
	if (root.isMember(key_gameObjectHide))
	{
		bool bHide = root[key_gameObjectHide].asBool();

		if (bHide)
		{
			HideGameObject();
			objectCollision = CollisionChannels::NoCollision;
		}
		else
		{
			ShowGameObject();
			Sound* sound = SoundManager::Get()->GetSound("Sound/Boat/Cannon.wav");
			sound->PlaySound();

			if (root.isMember("spPo"))
			{
				const Json::Value& boatPositionArray = root["spPo"];

				Json::StreamWriterBuilder writerBuilder;
				std::string msgToSend = Json::writeString(writerBuilder, boatPositionArray);

				sf::Vector2f boatPosition = sf::Vector2f(boatPositionArray[0].asFloat(), boatPositionArray[1].asFloat());
				std::cout << "BoatPosition x: " << boatPosition.x << " - y: " << boatPosition.y << std::endl;
			}
			if (root.isMember("spRo"))
			{
				float rot = root["spRo"].asFloat();
				SetRotation(rot);
			}
		}
	}
	if (root.isMember(key_gameObjectRot))
	{
		float rot = root[key_gameObjectRot].asFloat();
		SetRotation(rot);
	}
	//Set position
	if (root.isMember(key_gameObjectPosition))
	{
		const Json::Value& boatPositionArray = root[GameObject::key_gameObjectPosition];
		sf::Vector2f boatPosition = sf::Vector2f(boatPositionArray[0].asFloat(), boatPositionArray[1].asFloat());
		SetPosition(boatPosition);
	}
}

void Bullet::SetBulletSpeed(float bulletSpeed)
{
	this->bulletSpeed = bulletSpeed;
}

void Bullet::OnCollisionEnter(GameObject* otherGO)
{
	if (otherGO)
	{
		objectCollision = CollisionChannels::NoCollision;
		HideGameObject();

	}
}
