#pragma once
#include "GameObject.h"

class BoatBullet : public GameObject
{
public: 
	BoatBullet(GameObjectInitialInfo initialInfo);

	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

	//Function that gets called each tick
	virtual void Update();

private: 
	//speed the bullet goes
	float bulletSpeed; 
};