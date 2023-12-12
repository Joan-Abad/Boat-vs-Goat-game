#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public: 
	Bullet();
	Bullet(GameObjectInitialInfo initialInfo);

	//Function that gets called each tick
	virtual void Update();

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root) override;

private: 
	//speed the bullet goes
	float bulletSpeed; 

	
};