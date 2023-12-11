#include "GameObjects/BoatBullet.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/TextureManager.h"
#include "ApplicationHelper.h"

BoatBullet::BoatBullet(GameObjectInitialInfo initialInfo) : GameObject(initialInfo), bulletSpeed(2400.f)
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

void BoatBullet::Draw(sf::RenderWindow& window)
{
	window.draw(initialSprite);
}

void BoatBullet::Update()
{
	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		//Accelerate boat bullet on forward vector direction
		SetPosition(GetPosition() + forwardVector * bulletSpeed * ApplicationHelper::GetDeltaTime());
	}
}
