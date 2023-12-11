#pragma once
#include <json.h>
#include <SFML/Graphics.hpp>

struct Transform
{
	Transform(sf::Vector2f position, float angle, sf::Vector2f scale)
	{
		this->position = position;
		this->Angle = angle; 
		this->scale = scale; 
	}

	sf::Vector2f position;
	float Angle; 
	sf::Vector2f scale; 
};

class GameObjectInitialInfo 
{
public:
	GameObjectInitialInfo(sf::Vector2f playerPosition, float angle)
	{
		this->playerPosition = playerPosition;
		this->angle = angle;
	}
	sf::Vector2f playerPosition;
	float angle;
};

class GameObject
{
public:
	GameObject(GameObjectInitialInfo gameObjectInitialInfo);

	//Set true if we want this cass to replicate the data 
	bool bReplicates;
	bool bReplicateTransform;

	virtual void Init();

	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

	//Function that gets called each tick
	virtual void Update();

	//Called after update
	virtual void EndUpdate();

	//Sets the transform of the actor
	void SetGameObjectTransform(sf::Vector2f position = { 0,0 }, float angle = 0, sf::Vector2f scale = { 0,0 });

	//Gets the transform of the actor
	Transform GetActorTransform();

	//Sets the position in the screen of the initial sprite
	void SetPosition(sf::Vector2f newPosition);

	//Sets the rotation in the screen of the initial sprite
	void SetRotation(float angle);

	//Gets the rotation of the initial sprite in angles
	float GetRotation();

	//Changes the scale of the initial sprite
	void SetScale(sf::Vector2f newScale);

	//Gets the scale of the character
	sf::Vector2f GetScale();

	//Gets the rotation of the player sprite in angles
	sf::Vector2f GetPosition();

	//Sprite of the player
	sf::Sprite initialSprite;

	//The forward vector of the player
	sf::Vector2f forwardVector;

	//The right vector of the player
	sf::Vector2f rightVector;
protected: 

	//Json root value. We should add here all the information we want to send by network
	Json::Value localRootData;
};