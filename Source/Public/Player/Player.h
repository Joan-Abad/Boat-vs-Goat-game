#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <json.h>
#include <unordered_map>

class NetworkingManager; 
class InputAction;

struct PlayerInitialInfo
{
	PlayerInitialInfo(unsigned short uniqueID, sf::Vector2f playerPosition, float angle, const char* playerTexturePath)
	{
		this->uniqueID = uniqueID;
		this->playerPosition = playerPosition;
		this->angle = angle; 
		this->playerTexturePath = playerTexturePath;
	}
	unsigned short uniqueID;
	sf::Vector2f playerPosition;
	float angle;
	const char* playerTexturePath; 
};

//A representation of the game object + its input as it is an easy game
class Player
{
	friend class AppManager; 
	friend class NetworkingManager;
public:

	Player(sf::Window & window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo);
	virtual ~Player();
	
	//Always call this function for window responsiveness from child classes
	virtual void HandlePlayerInput();

	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

	//Gets the rotation of the player sprite in angles
	float GetRotation();

	//Function that gets called each tick
	virtual void Update();

	//Called after update
	virtual void EndUpdate();

	//Sets the position in the screen of the player
	void SetPosition(sf::Vector2f newPosition);

	//Sets the rotation in the screen of the player
	void SetRotation(float angle);

	NetworkingManager* GetNetworkingManager();

protected: 

	template<typename FncAdd, typename FncObject>
	auto BindAction(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
	{
		return std::bind(FunctionAddress, ObjectOwningFunction);
	}

	inline std::vector<InputAction*>& GetPlayerActions(){ return playerActions; };
	
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

	
	void UpdaetPlayerInfo(const std::string& NetworkData);

	void AddLocalNetworkDataToRootData();

	
	//Json root value. We should add here all the information we want to send by network
	Json::Value localRootData;

	
	
	//Check if this player should handle input by this process
	bool bIsLocallyControlled;

	//Stores all the player actions 
	std::vector<InputAction*> playerActions; 
	std::unordered_multimap<sf::Keyboard::Key, InputAction*> playerActionsMulti; 
protected: 
	

	//The forward vector of the player
	sf::Vector2f forwardVector;
	//The right vector of the player
	sf::Vector2f rightVector;

	//Windows window where the player lives
	sf::Window* window; 

	//Texture of the player
	sf::Texture playerTexture; 

	//Sprite of the player
	sf::Sprite playerSprite; 
};
