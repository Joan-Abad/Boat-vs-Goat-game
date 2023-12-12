#pragma once
#include "GameObjects/GameObject.h"

class Boat; 

class BoatLifes : public GameObject
{
public: 
	BoatLifes(GameObjectInitialInfo initialInfo, Boat* associatedBoat);

protected: 
	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

private: 
	Boat* associatedBoat; 

	sf::Sprite heart; 
	sf::Text lifeText; 
	int fontSize; 
};