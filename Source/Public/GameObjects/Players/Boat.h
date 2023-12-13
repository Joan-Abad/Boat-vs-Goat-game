#pragma once

#include "GameObjects/Players/Player.h"
#include "GameObjects/Bullet.h"

#define PLAYER1TEXTPATH "Art/boat/PNG/Boats_color1/Boat_color1_1.png"
#define PLAYER2TEXTPATH "Art/boat/PNG/Boats_color3/Boat_color3_1.png"
#define MaxBulletsPerBoatOnScreen 6

class Sound;  

class Boat : public Player
{
	friend class NetworkingManagerClient;
	friend class NetworkingManagerServer; 
public: 
	Boat(GameObjectInitialInfo playerInitialInfo, bool PlayerPlayable);

	virtual void Init() override;
	//Function that gets called each tick
	virtual void Update() override;
	void SetIsAccelerating(bool bIsAcelerating);
	void SetIsRotatingLeft(bool bRotatingLeft);
	void SetIsRotatingRight(bool bRotatingRight);
	void SetIsShooting(bool bIsShooting);

	//Triggered when a collision is detected with another game object
	virtual void  OnCollisionEnter(GameObject* otherGO);

	//Triggered when a collision is colliding with another game object
	virtual void  OnColliding(GameObject* otherGO);

	//Triggered when a collision finished collising with another game object
	virtual void  OnCollissionExit(GameObject* otherGO);

	static unsigned short boatCounter;

private:
	//Boat mechanics
	void StartAccelerateBoat();
	void AccelerateBoat();
	void StopAccelerateBoat();

	//Not used for now
	void DecelerateBoat();

	void StartRotateBoatLeft();
	void RotateBoatLeft();
	void StopRotateBoatLeft();

	void StartShootBullet();
	void BoatShootBullet();
	void StopShootBullet();

	void StartRotateBoatRight();
	void RotateBoatRight();
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

	std::array<Bullet*, MaxBulletsPerBoatOnScreen> bullets;
	static const char* key_AccelerateBoatID;
	static const char* key_RotateBoatLeftID;
	static const char* key_RotateBoatRightID;
	static const char* key_ShootBoatID;

	static const uint32_t key_accelerateBoat;
};