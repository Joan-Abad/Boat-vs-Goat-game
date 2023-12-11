#include "GameObjects/Bullet.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/TextureManager.h"
#include "ApplicationHelper.h"

Bullet::Bullet() : GameObject()
{
	bTickEnabled = false;
}

Bullet::Bullet(GameObjectInitialInfo initialInfo) : GameObject(initialInfo), bulletSpeed(2400.f)
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

void Bullet::Draw(sf::RenderWindow& window)
{
	window.draw(initialSprite);
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
			//Accelerate boat bullet on forward vector direction
			SetPosition(GetPosition() + forwardVector * bulletSpeed * ApplicationHelper::GetDeltaTime());
	}
}
