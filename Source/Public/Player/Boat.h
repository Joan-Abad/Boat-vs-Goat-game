#pragma once
#include "Player/Player.h"

#define PLAYER1TEXTPATH "Art/boat/PNG/Boats_color1/Boat_color1_1.png"
#define PLAYER2TEXTPATH "Art/boat/PNG/Boats_color3/Boat_color3_1.png"

class Boat : public Player
{
public: 
	Boat(sf::Window& window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo);

	virtual void HandlePlayerInput();

private:
	//Boat mechanics
	void AccelerateBoat();
	void DecelerateBoat();
	void RotateBoatLeft();
	void RotateBoatRight();

private: 
	//How fast the boat accelerates each frame
	sf::Vector2f boatAcceleration;
	//The speed of the boat is frame
	float speed;
	//Angle the boat turns around each second
	float angleBoatSpeedEachSecond;

	//Input Actions
	InputAction action_Space;
	InputAction action_P;
	InputAction action_W;
	InputAction action_S;
	InputAction action_RotateLeft;
	InputAction action_RotateRight;

};