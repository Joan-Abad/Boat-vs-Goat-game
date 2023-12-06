#pragma once
#include <functional>

#include "SFML/Window.hpp"

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

class PlayerController
{
public:
	PlayerController();

};