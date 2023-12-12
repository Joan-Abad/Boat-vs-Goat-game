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
	GameObjectInitialInfo() //: playerPosition{ 0.0f, 0.0f }, angle(0.f), scale({1.0f, 1.0f})
	{
		this->playerPosition = {0.0f, 0.0f};
		this->angle = 0.f;
		this->scale = {1.0f, 1.0f};
	};
	GameObjectInitialInfo(sf::Vector2f playerPosition, float angle, sf::Vector2f scale = {1.0f,1.0f})
	{
		this->playerPosition = playerPosition;
		this->angle = angle;
		this->scale = scale; 
	}
	sf::Vector2f playerPosition;
	float angle;
	sf::Vector2f scale; 
};

class GameObject
{
public:
	GameObject();
	GameObject(GameObjectInitialInfo gameObjectInitialInfo);

	//Set true if we want this cass to replicate the data 
	bool bReplicates;
	bool bReplicateTransform;
	bool bTickEnabled; 

	//Adds the data that will be send at the end of the frame
	//Value needs to be a supported JSON type (string, number, Json Object, array, bool or null)
	template <typename Value>
	void AddLocalNetworkDataToSend(const char* KEY, Value valueToSend)
	{
		if (!gameObjectNetData.isMember(key_gameObjectID))
			gameObjectNetData[key_gameObjectID] = gameObjectID;

		gameObjectNetData[KEY] = valueToSend;
	}

	virtual void Init();

	//Destroys all the resources of the actor and removes it from the level
	void DestroyActor();

	//Will draw the actor 
	void ShowGameObject();

	//Hides the visual part of the actor 
	void HideGameObject();

	bool GameObjectOutOfScreen();

	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

	//Function that gets called each tick
	virtual void Update();

	//Called after update
	virtual void EndUpdate();

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value &root);

	//Recived data from a client. Extract any information needed
	virtual void UpdateServerData(const Json::Value& root);

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

	//Gets the current map being played 
	class Map* GetCurrentMap();

	//Sprite of the player
	sf::Sprite initialSprite;



	static const char* key_gameObjectPosition;
	static const char* key_gameObjectRot;
	static const char* key_gameObjectID;
	static const char* key_gameObjectHide; 
	//GETTERS
	inline int GetGameObjectIDTracker() { return gameObjectIDTracker; };
	inline int GetGameObjectID() { return gameObjectID; };
	inline sf::Vector2f GetForwardVector() { return forwardVector; };
	inline sf::Vector2f GetRightVector() { return rightVector; };
protected: 

	//Passes the game object network data to the NetworkManager
	void AddGameObjectNetDataToManagerNetData();

	//Json root value. We should add here all the information we want to send by network
	Json::Value gameObjectNetData;

private: 
	
	//The forward vector of the player
	sf::Vector2f forwardVector;

	//The right vector of the player
	sf::Vector2f rightVector;

	//Gives a unique id to the spawning game object
	static int gameObjectIDTracker;

	//The unique id of this game object
	int gameObjectID;
};