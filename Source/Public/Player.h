#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

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
	FOnKeyTriggered OnKeyTriggered;
	//Triggered when a key is being pressed
	FOnKeyOnGoing OnKeyOnGoing;
	//Triggered when a key is released
	FOnKeyReleased OnKeyReleased;
	
	//Setters
	void SetKey(sf::Keyboard::Key key);
	void SetIsKeyPressed(bool keyPressed);

	//Getters
	inline bool GetIsKeyPressed() { return bIsKeyPressed; };
	inline sf::Keyboard::Key GetKey() { return key; };
private:
	sf::Keyboard::Key key;
	
	//Tracks the space key state
	bool bIsKeyPressed;
};

//A representation of the game object + its input as it is an easy game
class Player
{

public:
	Player(sf::Window & window);

	void HandlePlayerInput(float DeltaSeconds);

	void CheckKeyPressed(InputAction& inputAction);
private: 
	void PrintPressed();
	void PrintRndomMessage();
	void PrintReleased();
	void SendTestPacket();

	template<typename FncAdd, typename FncObject>
	auto BindAction(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
	{
		return std::bind(FunctionAddress, ObjectOwningFunction);
	}
	
	InputAction action_Space;
	InputAction action_P;

	//The total life of the player
	int playerLifes;  

	//Windows window where the player lives
	sf::Window* window; 

	//Texture of the player
	sf::Texture playerTexture; 

	//Sprite of the player
	sf::Sprite playerSprite; 
};
