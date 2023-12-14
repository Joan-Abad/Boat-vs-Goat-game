#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public: 
	Bullet();
	Bullet(GameObjectInitialInfo initialInfo);

	//Function that gets called each tick
	virtual void Update(float deltaTime);

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root) override;

protected: 

	//Triggered when a collision is detected with another game object
	virtual void  OnCollisionEnter(GameObject* otherGO);

private: 
	//speed the bullet goes
	float bulletSpeed; 

	
};