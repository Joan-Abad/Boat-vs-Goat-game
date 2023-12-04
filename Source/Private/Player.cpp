#include "Player.h"
#include <iostream>
#include <SFML/Network.hpp>
#include "NetworkingManager.h"
#include "NetworkingManagerClient.h"

Player::Player(sf::Window& window) : playerLifes(3)
{
	this->window = &window;

	action_Space.SetKey(sf::Keyboard::Key::Space);
	auto address = &Player::PrintPressed;
	action_Space.OnKeyTriggered = BindAction(&Player::PrintPressed, this);
	action_Space.OnKeyOnGoing = BindAction(&Player::PrintRndomMessage, this);
	action_Space.OnKeyReleased = BindAction(&Player::PrintReleased, this);

	action_P.SetKey(sf::Keyboard::Key::P);
	action_P.OnKeyTriggered = BindAction(&Player::SendTestPacket, this);
}

void Player::HandlePlayerInput(float DeltaSeconds)
{
	sf::Event inputEvent;
	while (window->pollEvent(inputEvent))
	{
		if (inputEvent.type == sf::Event::Closed)
			window->close();
	}
	
	CheckKeyPressed(action_Space);
	CheckKeyPressed(action_P);
}

void Player::CheckKeyPressed(InputAction& inputAction)
{
	bool bIsSpacePressedThisFrame = sf::Keyboard::isKeyPressed(inputAction.GetKey());

	if (bIsSpacePressedThisFrame)
	{
		//On Trigger
		if (!inputAction.GetIsKeyPressed())
		{
			inputAction.SetIsKeyPressed(true);
			if (inputAction.OnKeyTriggered)
				inputAction.OnKeyTriggered(); 
		}
		//On Going
		if(inputAction.OnKeyOnGoing)
			inputAction.OnKeyOnGoing();
	}
	else
	{
		//On Released
		if (inputAction.GetIsKeyPressed())
		{
			if (inputAction.OnKeyReleased)
				inputAction.OnKeyReleased();
		}
		inputAction.SetIsKeyPressed(false);
	}
}

void Player::PrintPressed()
{
	std::cout << "On pressed\n";
}

void Player::PrintReleased()
{
	std::cout << "On released\n";
}

void Player::SendTestPacket()
{
	sf::Packet packet;
	std::string msgToSend = "Bon dia";
	packet << msgToSend;

	NetworkingManager* netManager = &NetworkingManager::GetNetworkingManager<NetworkingManagerClient>();
	netManager->AddPacketToSend(packet);
}

void Player::PrintRndomMessage()
{
	std::cout << "Print random mensaje brou\n";
}


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
