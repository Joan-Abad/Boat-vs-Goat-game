#include "Managers/AppManager.h"
#include <iostream>
#include "Managers/Networking/NetworkingManager.h"
#include "Managers/Networking/NetworkingManagerServer.h"
#include "Managers/Networking/NetworkingManagerClient.h"
#include "Map/Map.h"
#include "Window.h"
#include "Managers/GameManager.h"

AppManager* AppManager::instance = nullptr;


AppManager::AppManager() : playerType(EPlayerType::ENotAssigned)
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
	std::cout << "Do you want to be the server +or the client? 0 for server and 1 client\n";

	//Assign server or client
	while(playerType == EPlayerType::ENotAssigned)
	{
		int response = -1;

		std::cout << "Response: ";
		std::cin >> response;

		if (response == 0)
			playerType = EPlayerType::EServer;
		else if (response == 1)
			playerType = EPlayerType::EClient;
		else
			std::cout << "Reponse is not 0 or 1. Please choose correctly between server or client\n";
	}

	if (playerType == EPlayerType::EServer)
	{
		//Initialize Network Manager + change screen to game waiting players
		//Connect directly to the SERVER_IP
		NetworkManager = new NetworkingManagerServer();
	}
	else if (playerType == EPlayerType::EClient)
	{
		std::string IP;
		unsigned short port; 
		if (FORCE_SERVER_IP)
		{
			IP = SERVER_IP;
		}
		else
		{
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
			
			
		}
		std::cout << "Connecting to: " << IP << std::endl;

		NetworkManager = new NetworkingManagerClient(IP, port);
	}

	if (NetworkManager)
		NetworkManager->OnInit();
}

void AppManager::Update()
{
	while (!bCloseGame)
	{
		NetworkManager->UpdateNetworkData();

		gameManager->Update();
	}
}

void AppManager::CloseGame()
{

}
