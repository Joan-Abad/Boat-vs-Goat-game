#pragma once
#include <vector>
#include <SFML/Window.hpp>
#include "Managers/GameManager.h"
#include <unordered_set>
#include <functional>
#include <unordered_map>

class Window; 

class InputAction
{
	friend class InputManager; 

	//On Key Pressed fnc
	using FOnKeyTriggered = std::function<void()>;
	//On Key OnGoing fnc
	using FOnKeyOnGoing = std::function<void()>;
	//On Key Released fnc
	using FOnKeyReleased = std::function<void()>;
public:

	//Triggered when a key is pressed
	std::vector<FOnKeyTriggered> OnKeyTriggered;
	//Triggered when a key is being pressed
	std::vector<FOnKeyOnGoing> OnKeyOnGoing;
	//Triggered when a key is released
	std::vector<FOnKeyReleased> OnKeyReleased;

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

class InputManager
{
public:
	static InputManager* GetInputManager()
	{
		if (!instance)
		{
			instance = new InputManager();
			instance->window = GameManager::GetGameManager()->GetGameWindow();

		}

		return instance; 
	}

	void Update();
	std::unordered_set<sf::Keyboard::Key>& GetKeysPressed() { return KeysPressed; };
	InputAction* CreateInputAction(sf::Keyboard::Key key);
	InputAction* GetInputAction(sf::Keyboard::Key key);
private: 
	InputManager();
	~InputManager();
	std::unordered_set<sf::Keyboard::Key> KeysPressed;
	Window* window; 
	std::vector<InputAction*> ProjectInputActions; 
	static InputManager* instance; 
	std::unordered_map<sf::Keyboard::Key, InputAction*> keyInputActionMappings; 
};