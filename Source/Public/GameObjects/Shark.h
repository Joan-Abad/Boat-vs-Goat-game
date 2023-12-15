#pragma once
#include "GameObjects/GameObject.h"

class Shark : public GameObject
{
	friend class Lake; 
public: 
	Shark(sf::Vector2f initLocation, float radius);


	//Function that gets called each tick
	virtual void Update(float deltaTime);


	void SetNewLocationToMove();

	float FindRotationDirection(sf::Vector2f startingLocation);

	static sf::Texture* sharkTexture; 
	static const char* key_spawnSharkNoise;

protected: 
	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root);
private: 
	float timeToReachLocation;
	float alphaTracker; 
	float radius; 
	sf::Vector2f initLocation; 
	sf::Vector2f lastLocation;
	sf::Vector2f destinationLocation;

};