#pragma once
#include "GameObjects/GameObject.h"

class Boat; 

class BoatLifes : public GameObject
{
	friend class Boat; 
public: 
	BoatLifes(GameObjectInitialInfo initialInfo, Boat* associatedBoat);
	
protected: 
	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);
	void UpdateLifeText();
private: 
	Boat* associatedBoat; 

	sf::Sprite heart; 
	sf::Text lifeText;	
	int fontSize; 
};