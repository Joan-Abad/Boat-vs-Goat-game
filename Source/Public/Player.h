#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

#define PLAYER1TEXTPATH "Art/boat/PNG/Boats_color1/Boat_color1_1.png"
#define PLAYER2TEXTPATH "Art/boat/PNG/Boats_color3/Boat_color3_1.png"

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
	Player(sf::Window & window, bool PlayerPlayable, const char* texturePath);

	void HandlePlayerInput();

	void CheckKeyPressed(InputAction& inputAction);

	void Draw(sf::RenderWindow& window);

	float GetRotation();
	void SetPosition(sf::Vector2f newPosition);
	void SetRotation(float angle);
private: 
	void PrintPressed();
	void PrintRndomMessage();
	void PrintReleased();
	void SendTestPacket();

	void AccelerateBoat();
	void DecelerateBoat();
	void RotateBoatLeft();
	void RotateBoatRight();

	sf::Vector2f forwardVector; 
	sf::Vector2f rightVector; 
	
	sf::Vector2f boatAcceleration;

	float speed; 
	float angleBoatSpeedEachSecond; 

	template<typename FncAdd, typename FncObject>
	auto BindAction(FncAdd FunctionAddress, FncObject ObjectOwningFunction)
	{
		return std::bind(FunctionAddress, ObjectOwningFunction);
	}
	
	InputAction action_Space;
	InputAction action_P;
	InputAction action_W;
	InputAction action_S;
	InputAction action_RotateLeft;
	InputAction action_RotateRight;

	//Check if this player should handle input by this process
	bool IsPlayerPlayable;

	//The total life of the player
	int playerLifes;  

	//Windows window where the player lives
	sf::Window* window; 

	//Texture of the player
	sf::Texture playerTexture; 

	//Sprite of the player
	sf::Sprite playerSprite; 
};
