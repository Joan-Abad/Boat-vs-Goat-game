#include "Window.h"
#include "SFML/Network.hpp"
#include "Player.h"
#include "NetworkingManager.h"
#include "NetworkingManagerClient.h"
#include "NetworkingManagerServer.h"
#include "Map.h"
#include <iostream>
#include "ApplicationHelper.h"

#define SCREEN_WIDTH 1920.f
#define SCREEN_HEIGHT 1080.f

enum class TypePlayer
{
    Server,
    Client,
    Error
};

bool bIsGameOpened = true; 
Map map("Art/Background/Water.jpg");
void DrawGame(sf::RenderWindow& window);
void InitNetworkingManager(NetworkingManager*& networkingManager, TypePlayer typePlayer);

int main()
{
    TypePlayer typePlayer;
    bool bCanPlayPlayer1, bCanPlayPlayer2;

#if _SERVER
    typePlayer = TypePlayer::Server;
    bCanPlayPlayer1 = true;
    bCanPlayPlayer2 = false;
#elif _CLIENT
    typePlayer = TypePlayer::Client;
    bCanPlayPlayer1 = false;
    bCanPlayPlayer2 = true;
#else
    typePlayer = TypePlayer::Error;
#endif

    //Networking Manager
    NetworkingManager* networkingManager = nullptr;
    InitNetworkingManager(networkingManager, typePlayer);


    //Game update
    Window* window = new Window("Boat vs Goat", sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    if (!window)
        return -1; 

    window->GetWindow().setFramerateLimit(60);
    Player* player1 = new Player(window->GetWindow(), bCanPlayPlayer1, PLAYER1TEXTPATH);
    Player* player2 = new Player(window->GetWindow(), bCanPlayPlayer2, PLAYER2TEXTPATH);
    player1->SetPosition({ SCREEN_WIDTH / 8.f , SCREEN_HEIGHT / 2.5f});
    player1->SetRotation(-90.f);
    player2->SetPosition({ SCREEN_WIDTH * 0.8f , SCREEN_HEIGHT / 2.5f });
    player2->SetRotation(90.f);

    while (bIsGameOpened)
    {
        ApplicationHelper::SetDeltaTime();

        //Recieve networking info
        networkingManager->ListenIncomingPackages();

        //TODO: Change. Not the best way but will do for now
#if _SERVER
        //Input
        player1->HandlePlayerInput();
#elif _CLIENT
        player2->HandlePlayerInput();
#endif
        

        //Send networking packages
        networkingManager->SendPackages();
        
        //Drawing
        window->ClearWindow();
        DrawGame(window->GetWindow());
        player1->Draw(window->GetWindow());
        player2->Draw(window->GetWindow());
        window->Draw();
    }

    //Finish game
    delete window; 
    return 0;
}

void InitNetworkingManager(NetworkingManager*& _networkingManager, TypePlayer typePlayer)
{
    if (typePlayer == TypePlayer::Server)
    {
        _networkingManager = &NetworkingManager::GetNetworkingManager<NetworkingManagerServer>();
        _networkingManager->Bind(53001);
    }
    else
    {
        _networkingManager = &NetworkingManager::GetNetworkingManager<NetworkingManagerClient>();
        _networkingManager->Bind(53001);
    }
}

void DrawGame(sf::RenderWindow& window)
{
    map.DrawWap(window);
    
}