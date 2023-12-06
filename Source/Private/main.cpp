#include "Window.h"
#include "SFML/Network.hpp"
#include "Player.h"
#include "NetworkingManager.h"
#include "NetworkingManagerClient.h"
#include "NetworkingManagerServer.h"
#include "Map.h"
#include <iostream>
#include "ApplicationHelper.h"
#include <json.h>
#include "GameManager.h"

int main()
{
    GameManager* gameManager = GameManager::GetGameManager();

    gameManager->InitGame();
    gameManager->Update();
    gameManager->CloseGame();

    delete gameManager; 
    return 0; 
}