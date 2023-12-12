#include "GameObjects/Bullet.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/TextureManager.h"
#include "ApplicationHelper.h"
#include "Managers/SoundManager.h"

Bullet::Bullet() : GameObject()
{
	bTickEnabled = false;
}

Bullet::Bullet(GameObjectInitialInfo initialInfo) : GameObject(initialInfo), bulletSpeed(1200.f)
{
	/*const char* bulletImagePath = "Art/bullet.png";

	if (!TextureManager::GetTextureManager().HasTexture(bulletImagePath))
	{
		if (TextureManager::GetTextureManager().AddTexture(bulletImagePath))
			initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture(bulletImagePath));
	}*/
	initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture("Art/bullet.png"));
	initialSprite.setScale(initialSprite.getScale() / 20.f);
}

void Bullet::Update()
{
	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		//Stop updating the object if it is out of screen
		if (GameObjectOutOfScreen())
		{
			bTickEnabled = false;
			HideGameObject();
		}
		else
		{
			//Accelerate boat bullet on forward vector direction
			sf::Vector2f bulletPosition = GetPosition() + GetForwardVector() * bulletSpeed * ApplicationHelper::GetDeltaTime();
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
			HideGameObject();
		else
		{
			ShowGameObject();
			Sound* sound = SoundManager::Get()->GetSound("Sound/Boat/blaster.wav");
			sound->PlaySound();
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
