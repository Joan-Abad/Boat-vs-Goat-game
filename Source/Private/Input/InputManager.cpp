#include "Input/InputManager.h"
#include "Managers/AppManager.h"
#include "Managers/GameManager.h"
#include "Window.h"

InputManager* InputManager::instance = nullptr;

void InputManager::Update()
{
	sf::Event inputEvent;
	sf::RenderWindow& renderWindow = window->GetWindow();
	while (renderWindow.pollEvent(inputEvent))
	{
		if (inputEvent.type == sf::Event::Closed)
			renderWindow.close();

		if (inputEvent.type == sf::Event::KeyPressed)
		{
			auto pair = KeysPressed.insert(inputEvent.key.code);
			if(pair.second)
				std::cout << "Key added: " << inputEvent.key.code << std::endl;
		}
		else if (inputEvent.type == sf::Event::KeyReleased)
		{
			KeysPressed.erase(inputEvent.key.code);
			std::cout << "Key removed: " << inputEvent.key.code << std::endl;
		}


	}
}
