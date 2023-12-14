#pragma once
#include "GameObjects/GameObject.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <json.h>
#include <unordered_map>

class NetworkingManager; 
class InputAction;

//A representation of the game object + its input
class Player : public GameObject
{
	friend class AppManager;
	friend class NetworkingManager;
public:

	Player( GameObjectInitialInfo info, bool isLocallyController);
	virtual ~Player();

	//Function that gets called each tick
	virtual void Update(float deltaTime);

	NetworkingManager* GetNetworkingManager();
	inline int GetPlayerID() { return playerID; };
protected:

	template<typename FncAdd, typename FncObject>
	auto BindAction(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
	{
		return std::bind(FunctionAddress, ObjectOwningFunction);
	}

	static int playerTrackerID;

	int playerID;

private:


	//Check if this player should handle input by this process
	bool bIsLocallyControlled;
};
