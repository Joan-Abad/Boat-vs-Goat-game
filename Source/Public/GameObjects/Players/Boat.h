#pragma once
#include "GameObjects/Players/Player.h"

#define PLAYER1TEXTPATH "Art/boat/PNG/Boats_color1/Boat_color1_1.png"
#define PLAYER2TEXTPATH "Art/boat/PNG/Boats_color3/Boat_color3_1.png"

class Boat : public Player
{
	friend class NetworkingManagerClient;
	friend class NetworkingManagerServer; 
public: 
	Boat(sf::Window& window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo);

	virtual void HandlePlayerInput();

	virtual void Init() override;
	//Function that gets called each tick
	virtual void Update() override;
	void SetIsAccelerating(bool bIsAcelerating);
	void SetIsRotatingLeft(bool bRotatingLeft);
	void SetIsRotatingRight(bool bRotatingRight);
	void SetIsShooting(bool bIsShooting);
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
#if _DEBUG
	int timesAccelerated = 0; 
#endif
private: 
	bool bBoatIsShooting;
	bool bIsBoatAccelerating;
	bool bIsBoatRotatingLeft; 
	bool bIsBoatRotatingRight;

	//How fast the boat accelerates each frame
	sf::Vector2f boatAcceleration;
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

	float shootingCD;

	static const char* key_AccelerateBoatID;
	static const char* key_RotateBoatLeftID;
	static const char* key_RotateBoatRightID;
	static const char* key_ShootBoatID;
	static const char* key_boatPosition;
	static const char* key_boatAngle;
	static const char* key_boatID;
	static const uint32_t key_accelerateBoat;

};