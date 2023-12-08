#pragma once
#include <vector>
#include <SFML/Window.hpp>
#include "Managers/GameManager.h"
#include <unordered_set>

class Window; 

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
private: 
	InputManager() = default;
	std::unordered_set<sf::Keyboard::Key> KeysPressed;
	Window* window; 
	static InputManager* instance; 
};