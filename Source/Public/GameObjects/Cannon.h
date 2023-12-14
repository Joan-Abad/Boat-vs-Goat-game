#pragma once
#include "GameObjects/GameObject.h"
#include "Managers/SoundManager.h"

class Bullet; 

class Cannon : public GameObject
{
public: 
	Cannon(GameObjectInitialInfo initialInfo);

	void InitMissile(sf::Vector2f position, float angle);
	
	void ThrowCannonBall(Bullet& bullet);

	static Sound* cannonSound;

protected: 
	//Function that gets called each tick
	virtual void Update(float deltaTime);


	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root) override;

	//Triggered when a collision is detected with another game object
	virtual void  OnCollisionEnter(GameObject* otherGO);

private:
	sf::Vector2f tickAcceleration; 

	static const char* key_SpawnMissile;
	static const char* key_SpawnRot;
	static const char* key_SpawnPos;
	
	static float minCannonBallSpeed;
	static float maxCannonBallSpeed;
};