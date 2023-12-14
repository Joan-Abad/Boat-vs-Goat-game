#pragma once
#include "GameObjects/GameObject.h"

class Missile : public GameObject
{
public: 
	Missile(GameObjectInitialInfo initialInfo);

	void InitMissile(sf::Vector2f position, float angle);
protected: 
	//Function that gets called each tick
	virtual void Update(float deltaTime);

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root) override;

	//Triggered when a collision is detected with another game object
	virtual void  OnCollisionEnter(GameObject* otherGO);

private:
	//speed the bullet goes
	float missileSpeed;

	sf::Vector2f tickAcceleration; 

	static const char* key_SpawnMissile;
	static const char* key_SpawnRot;
	static const char* key_SpawnPos;

};