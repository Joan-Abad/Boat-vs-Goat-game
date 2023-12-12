#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public: 
	Bullet();
	Bullet(GameObjectInitialInfo initialInfo);

	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

	//Function that gets called each tick
	virtual void Update();

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root) override;

private: 
	//speed the bullet goes
	float bulletSpeed; 

	
};