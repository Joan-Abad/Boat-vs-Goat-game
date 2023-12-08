#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <json.h>
#include <unordered_map>

class NetworkingManager; 

class InputAction
{
	//On Key Pressed fnc
	using FOnKeyTriggered = std::function<void()>;
	//On Key OnGoing fnc
	using FOnKeyOnGoing = std::function<void()>;
	//On Key Released fnc
	using FOnKeyReleased = std::function<void()>;
public:

	//Triggered when a key is pressed
	FOnKeyTriggered OnKeyTriggered = nullptr;
	//Triggered when a key is being pressed
	FOnKeyOnGoing OnKeyOnGoing = nullptr;
	//Triggered when a key is released
	FOnKeyReleased OnKeyReleased = nullptr;

	static InputAction* CreateInputAction(std::vector<InputAction*>& playerActions, sf::Keyboard::Key key);

	bool CheckIfTriggerAction(sf::Keyboard::Key inComingKey);

	//Setters
	void SetKey(sf::Keyboard::Key key);
	void SetIsKeyPressed(bool keyPressed);

	//Getters
	inline bool GetIsKeyPressed() { return bIsKeyPressed; };
	inline sf::Keyboard::Key GetKey() { return key; };
	static std::vector<sf::Keyboard::Key> keysPressed; 
private:
	InputAction();
	InputAction(sf::Keyboard::Key key);

	//Key assigned to this action
	sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
	
	//Tracks the space key state
	bool bIsKeyPressed = false;

};

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
private: 

	
	void UpdaetPlayerInfo(const std::string& NetworkData);

	
	//Json root value. We should add here all the information we want to send by network
	Json::Value root; 

	
	
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
