#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <json.h>



class InputAction
{
	//On Key Pressed fnc
	using FOnKeyTriggered = std::function<void()>;
	//On Key OnGoing fnc
	using FOnKeyOnGoing = std::function<void()>;
	//On Key Released fnc
	using FOnKeyReleased = std::function<void()>;
public:
	InputAction();
	InputAction(sf::Keyboard::Key key);

	//Triggered when a key is pressed
	FOnKeyTriggered OnKeyTriggered = nullptr;
	//Triggered when a key is being pressed
	FOnKeyOnGoing OnKeyOnGoing = nullptr;
	//Triggered when a key is released
	FOnKeyReleased OnKeyReleased = nullptr;
	
	//Setters
	void SetKey(sf::Keyboard::Key key);
	void SetIsKeyPressed(bool keyPressed);

	//Getters
	inline bool GetIsKeyPressed() { return bIsKeyPressed; };
	inline sf::Keyboard::Key GetKey() { return key; };
private:
	//Key assigned to this action
	sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
	
	//Tracks the space key state
	bool bIsKeyPressed = false;
};

struct PlayerInitialInfo
{
	unsigned short uniqueID;
	sf::Vector2f playerPosition;
	float angle;
	const char* playerTexturePath; 
};

//A representation of the game object + its input as it is an easy game
class Player
{

public:
	Player(sf::Window & window, bool PlayerPlayable, PlayerInitialInfo playerInitialInfo);

	//Always call this function for window responsiveness from child classes
	virtual void HandlePlayerInput();

	//If you want to add more stuff to the character, override this function but also call the parent class to draw character sprite
	virtual void Draw(sf::RenderWindow& window);

	//Gets the rotation of the player sprite in angles
	float GetRotation();

	//Function that gets called each tick
	void Update();

	//Sets the position in the screen of the player
	void SetPosition(sf::Vector2f newPosition);

	//Sets the rotation in the screen of the player
	void SetRotation(float angle);

protected: 
	//Check if a key is pressed
	void CheckKeyPressed(InputAction& inputAction);

private: 

	
	void UpdaetPlayerInfo(const std::string& NetworkData);

	
	//Json root value. We should add here all the information we want to send by network
	Json::Value root; 

	template<typename FncAdd, typename FncObject>
	auto BindAction(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
	{
		return std::bind(FunctionAddress, ObjectOwningFunction);
	}
	
	//Check if this player should handle input by this process
	bool bIsLocallyControlled;

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