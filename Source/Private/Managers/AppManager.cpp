#include "Managers/AppManager.h"
#include <iostream>
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/Networking/NetworkingManagerServer.h"
#include "Managers/Networking/NetworkingManagerClient.h"
#include "Input/InputManager.h"

#include "Map/Map.h"
#include "Window.h"
#include "Managers/GameManager.h"
#include "ApplicationHelper.h"
AppManager* AppManager::instance = nullptr;


AppManager::AppManager()
{
	gameManager = GameManager::GetGameManager();
}

AppManager::~AppManager()
{
	delete gameManager;
}

void AppManager::InitGame()
{
	std::cout << "Welcome to boat vs goat! A game where you have to destoy the other boats to turn into the real goat\n";
	std::cout << "Do you want to be the server or the client? 0 for server and 1 client\n";


	int response = -1;
	//Assign server or client
	while (response != 0 && response != 1)
	{

		std::cout << "Response: ";
		std::cin >> response;

		if (response == 0)
		{
			//Initialize Network Manager + change screen to game waiting players
			//Connect directly to the SERVER_IP
			NetworkManager = new NetworkingManagerServer();
			NetworkManager->bIsServer = true;
		}
		else if (response == 1)
		{
			std::string IP;
			unsigned short port;

			port = NetworkingManager::GetGamePort();

			if (LAN)
			{
				IP = "127.0.0.1";
				std::cout << "Default port is 53001, your following response will be added to this port number. Example: ";
				unsigned short answer;
				std::cin >> answer;
				port += answer;
			}
			else
			{
				std::cout << "Now enter the IP of the server you want to connect: ";
				std::cin >> IP;
				std::cout << "Default port used: 53001";
			}

			std::cout << "Connecting to: " << IP << std::endl;

			NetworkManager = new NetworkingManagerClient(IP, port);
			NetworkManager->bIsServer = false;
		}
		else
			std::cout << "Reponse is not 0 or 1. Please choose correctly between server or client\n";
	}

	NetworkManager->OnInit();
}

void AppManager::Update()
{
	while (!bCloseGame)
	{
		NetworkManager->UpdateNetworkData();
		if (GameManager::GetGameManager()->bHasGameStarted)
		{
			//Shoulw be on app manager and not on applcation helper
			ApplicationHelper::SetDeltaTime();
			InputManager::GetInputManager()->Update();

			gameManager->Update();
		}
	}
}

void AppManager::CloseGame()
{

}
