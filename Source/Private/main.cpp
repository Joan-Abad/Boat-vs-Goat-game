#include "Managers/AppManager.h"

int main()
{
    AppManager* gameManager = AppManager::GetAppManager();

    gameManager->InitGame();
    gameManager->Update();
    gameManager->CloseGame();

    delete gameManager; 
    return 0; 
}