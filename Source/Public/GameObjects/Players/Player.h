#pragma once
#include "GameObjects/GameObject.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <json.h>
#include <unordered_map>

class NetworkingManager; 
class InputAction;

class PlayerInitialInfo : public GameObjectInitialInfo
{
public:
	PlayerInitialInfo(sf::Vector2f playerPosition, float angle) 
		: GameObjectInitialInfo(playerPosition, angle)
	{

	}

};

//A representation of the game object + its input
class Player : public GameObject
{
	friend class AppManager; 
	friend class NetworkingManager;
public:

	Player(bool isLocallyController, GameObjectInitialInfo info);
	virtual ~Player();

		//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
		virtual void Draw(sf::RenderWindow& window);

		//Function that gets called each tick
		virtual void Update();

	//Called after update
	virtual void EndUpdate();

	NetworkingManager* GetNetworkingManager();

protected: 

	template<typename FncAdd, typename FncObject>
	auto BindAction(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
	{
		return std::bind(FunctionAddress, ObjectOwningFunction);
	}
	
	//Adds the data that will be send at the end of the frame
	//Value needs to be a supported JSON type (string, number, Json Object, array, bool or null)
	template <typename Value>
	void AddLocalNetworkDataToSend(const char* KEY, Value valueToSend)
	{
			localRootData[KEY] = valueToSend;
	}

	static int playerTrackerID;
	
	int playerID;

private: 

	void AddLocalNetworkDataToRootData();

	//Check if this player should handle input by this process
	bool bIsLocallyControlled;
};
