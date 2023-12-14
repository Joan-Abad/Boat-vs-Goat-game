#pragma once

#include "GameObjects/Players/Player.h"
#include "GameObjects/Bullet.h"
#include "GameObjects/BoatLifes.h"

#define PLAYER1TEXTPATH "Art/PirateBoats/RedBoat.png"
#define PLAYER2TEXTPATH "Art/PirateBoats/BlueBoat.png"
#define PLAYER3TEXTPATH "Art/PirateBoats/GreenBoat.png"
#define PLAYER4TEXTPATH "Art/PirateBoats/YellowBoat.png"
#define MaxBulletsPerBoatOnScreen 6

class Sound;  

class Boat : public Player
{
	friend class NetworkingManagerClient;
	friend class NetworkingManagerServer; 
	friend class BoatLifes;
public: 
	Boat(GameObjectInitialInfo playerInitialInfo, bool PlayerPlayable);

	virtual void Init() override;
	//Function that gets called each tick
	virtual void Update(float deltaTime) override;
	void SetIsAccelerating(bool bIsAcelerating);
	void SetIsRotatingLeft(bool bRotatingLeft);
	void SetIsRotatingRight(bool bRotatingRight);
	void SetIsShooting(bool bIsShooting);

	void DisableBoat();

	inline int GetLifes() { return lifes; };

	static unsigned short boatCounter;

protected: 
	//Triggered when a collision is detected with another game object
	virtual void  OnCollisionEnter(GameObject* otherGO);

	//Triggered when a collision is colliding with another game object
	virtual void  OnColliding(GameObject* otherGO);

	//Triggered when a collision finished collising with another game object
	virtual void  OnCollissionExit(GameObject* otherGO);

private:
	//Boat mechanics
	void StartAccelerateBoat();
	void AccelerateBoat(float deltaTime);
	void StopAccelerateBoat();

	void StartRotateBoatLeft();
	void RotateBoatLeft(float deltaTime);
	void StopRotateBoatLeft();

	void StartShootBullet();
	void BoatShootBullet();
	void StopShootBullet();

	void StartRotateBoatRight();
	void RotateBoatRight(float deltaTime);
	void StopRotateBoatRight();

	void PrepareBullet(sf::Vector2f shootingLocation, float angle);

#if _DEBUG
	int timesAccelerated = 0; 
#endif

protected: 
	
	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root) override;

	//Recived data from a client. Extract any information needed
	virtual void UpdateServerData(const Json::Value& root);

private: 
	bool bBoatIsShooting;
	bool bIsBoatAccelerating;
	bool bIsBoatRotatingLeft; 
	bool bIsBoatRotatingRight;
	BoatLifes* boatLifeUI; 

	//How fast the boat accelerates each frame
	sf::Vector2f boatAcceleration;
	//Shooting socket
	sf::Vector2f GetShootingLocation();
	//The speed of the boat is frame
	float speed;
	//Angle the boat turns around each second
	float angleBoatSpeedEachSecond;

	//Input Actions
	//Wasted memory on objects that are not controlled by the player. With the creation of a player controller that just gets spawned for the application owner could solve this
	InputAction* action_W;
	InputAction* action_RotateLeft;
	InputAction* action_RotateRight;
	InputAction* action_ShootBoat;

	// Create a clock
	sf::Clock timer;

	Sound* shootingSound; 

	float shootingCD;
	int bulletTracker; 
	//The lifes the player has
	int lifes; 

public: 
	std::array<Bullet*, MaxBulletsPerBoatOnScreen> bullets;
	static const char* key_AccelerateBoatID;
	static const char* key_RotateBoatLeftID;
	static const char* key_RotateBoatRightID;
	static const char* key_ShootBoatID;
	static const char* key_UpdateBoatLife;
	static const char* key_bulletImpact;
	static const uint32_t key_accelerateBoat;
};