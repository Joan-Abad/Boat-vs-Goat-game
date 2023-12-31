#include "Input/InputManager.h"
#include "Managers/AppManager.h"
#include "Managers/GameManager.h"
#include "Window.h"

InputManager* InputManager::instance = nullptr;

InputAction::InputAction()
{
	
}

InputAction::InputAction(sf::Keyboard::Key key)
{
	this->key = key;
}

void InputAction::SetKey(sf::Keyboard::Key key)
{
	this->key = key;
}

void InputAction::SetIsKeyPressed(bool keyPressed)
{
	bIsKeyPressed = keyPressed;
}

InputManager::InputManager() : bInputEnabled(true)
{
	//Create project input actions
	CreateInputAction(sf::Keyboard::Key::W);
	CreateInputAction(sf::Keyboard::Key::A);
	CreateInputAction(sf::Keyboard::Key::D);	
	CreateInputAction(sf::Keyboard::Key::Space);

}

InputManager::~InputManager()
{
	for (auto projectInputAction : ProjectInputActions)
		delete projectInputAction;
}

void InputManager::Update()
{
	/*bool isGameClosed = AppManager::GetAppManager()->GetIsGameClosed();
	while (!isGameClosed)
	{*/
		sf::Event inputEvent;
		sf::RenderWindow& renderWindow = window->GetWindow();
		auto threadID = std::this_thread::get_id();

		//std::cout << "Checking input from thread: " << threadID << std::endl; 
		while (renderWindow.pollEvent(inputEvent))
		{
			if (inputEvent.type == sf::Event::Closed)
				renderWindow.close();

			if (!bInputEnabled)
				return;

			if (inputEvent.type == sf::Event::KeyPressed)
			{
				auto pair = KeysPressed.insert(inputEvent.key.code);
				//if (pair.second)
					//std::cout << "Key added: " << inputEvent.key.code << std::endl;

				auto keyInputActionMapping = keyInputActionMappings[inputEvent.key.code];

				if (keyInputActionMapping)
				{
					for (auto TriggerFunction : keyInputActionMapping->OnKeyTriggered)
						TriggerFunction();
				}
			}
			else if (inputEvent.type == sf::Event::KeyReleased)
			{
				KeysPressed.erase(inputEvent.key.code);
				//std::cout << "Key removed: " << inputEvent.key.code << std::endl;

				auto keyInputActionMapping = keyInputActionMappings[inputEvent.key.code];

				if (keyInputActionMapping)
				{
					for (auto TriggerFunction : keyInputActionMapping->OnKeyReleased)
						TriggerFunction();
				}
			}


		}

		//Trigger on going fncs
		for (auto key : KeysPressed)
		{
			auto keyInputAction = keyInputActionMappings[key];

			if (keyInputAction)
				for (auto OnGoingFnc : keyInputAction->OnKeyOnGoing)
					OnGoingFnc();

		}
	//}
}

void InputManager::EnableInput(bool bEnable)
{
	bInputEnabled = bEnable;
}

InputAction* InputManager::GetInputAction(sf::Keyboard::Key key)
{
	return keyInputActionMappings[key];
}

InputAction* InputManager::CreateInputAction(sf::Keyboard::Key key)
{
	auto findResult = keyInputActionMappings.find(key);

	//If the key was not in the table, create and add one
	if(findResult == keyInputActionMappings.end())
	{
		InputAction* newAction = new InputAction(key);
		if (newAction)
		{
			ProjectInputActions.push_back(newAction);
			keyInputActionMappings[key] = newAction;
		}
		else
			std::cerr << "Could not create the input action\n";

		return newAction;
	}
}

