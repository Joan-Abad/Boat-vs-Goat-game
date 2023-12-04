#include "Window.h"
#include "SFML/Network.hpp"
#include "Player.h"
#include "NetworkingManager.h"
#include "NetworkingManagerClient.h"
#include "NetworkingManagerServer.h"

enum class TypePlayer
{
    Server,
    Client,
    Error
};

bool bIsGameOpened = true; 

void InitNetworkingManager(NetworkingManager*& networkingManager, TypePlayer typePlayer);

int main()
{
    TypePlayer typePlayer;
    
#if _SERVER
    typePlayer = TypePlayer::Server;
#elif _CLIENT
    typePlayer = TypePlayer::Client;
#else
    typePlayer = TypePlayer::Error;
#endif

    //Networking Manager
    NetworkingManager* networkingManager = nullptr;
    InitNetworkingManager(networkingManager, typePlayer);


    //Game update
    Window* window = new Window("Boat vs Goat", sf::Vector2f(1920.f, 1080.f));
    if (!window)
        return -1; 

    Player player(window->GetWindow());

    while (bIsGameOpened)
    {
        player.HandlePlayerInput(0.f);
        networkingManager->ListenIncomingPackages();
        networkingManager->SendPackages();
        window->Update();
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