#include "Map/Map.h"
#include <iostream>
#include "Window.h"
#include "Managers/AppManager.h"
#include "Managers/Networking/NetworkingManager.h"

Map::~Map()
{
	for (auto& levelGameObject : levelGameObjects)
		delete levelGameObject.second;

}

void Map::UpdateMap()
{
	CheckCollisions();

	for (auto& goEntry: levelGameObjects)
	{
		GameObject& go = *goEntry.second;

		if (go.bTickEnabled)
		{
			go.Update();
			go.EndUpdate();
		}
	}
	EndUpdate();
}

void Map::EndUpdate()
{
	AddGameObjectNetDataToManagerNetData();
}

void Map::CheckCollisions()
{
	//Change to Map() UpdateMap function
	//Only check collisions on server
	if (AppManager::GetAppManager()->GetNetworkManager()->GetIsServer())
	{
		for (auto& pair1 : levelGameObjects) {
			if (!pair1.second->bCheckCollisions)
				continue;

			GameObject& firstGameObject = *pair1.second;

			for (auto& pair2 : levelGameObjects) {

				//avoid self collisions
				if (pair1 == pair2)
					continue;

				GameObject& secondGameObject = *pair2.second;

				//Ommit collisions checker
				if (!pair2.second->bCheckCollisions)
					continue;

				if (firstGameObject.objectCollision == CollisionChannels::NoCollision || secondGameObject.objectCollision == CollisionChannels::NoCollision)
					continue; 

				//Go through the collision and check if the other object should collide
				bool canCollide = false;
				for (CollisionChannels collChannel : firstGameObject.CollisionsToRespond)
				{
					if (collChannel == secondGameObject.objectCollision)
					{
						canCollide = true;
						break;
					}
				}

				if (!canCollide)
					continue;

				if (!firstGameObject.bIgnoreOwner)
				{
					if (firstGameObject.owner == &secondGameObject)
						continue;
				}

				bool actorIgnored = false;
				for (auto& actorToIgnore : firstGameObject.GameActorsToIgnoreCollision)
				{
					if (actorToIgnore == pair2.second)
					{
						actorIgnored = true;
						break;
					}
				}

				if (actorIgnored)
					continue;

				//If passed everything, do the actual collision checking
				sf::FloatRect bounds1 = firstGameObject.initialSprite.getGlobalBounds();
				sf::FloatRect bounds2 = pair2.second->initialSprite.getGlobalBounds();

				if (bounds1.intersects(bounds2)) {
					bool checkIfAlreadyColliding = false;

					for (auto& go : firstGameObject.goCollidingWith)
					{
						if (go == pair2.second)
						{
							//already colliding
							checkIfAlreadyColliding = true;
							break;
						}
					}

					//If its the first time colliding
					if (!checkIfAlreadyColliding)
					{
						firstGameObject.goCollidingWith.push_back(pair2.second);
						pair2.second->goCollidingWith.push_back(&firstGameObject);
						//On Colliding
						firstGameObject.OnCollisionEnter(pair2.second);
						pair2.second->OnCollisionEnter(&firstGameObject);
					}
					else
					{
						firstGameObject.OnColliding(pair2.second);
						pair2.second->OnColliding(&firstGameObject);
					}

				}
				else
				{
					//If they are not colliding, check if they where last frame to broadcast the on collision exit
					for (auto& go : pair1.second->goCollidingWith)
					{
						if (go == pair2.second)
						{
							std::vector<GameObject*>& gameObjectvec1 = pair1.second->goCollidingWith;
							std::vector<GameObject*>& gameObjectvec2 = pair2.second->goCollidingWith;
							gameObjectvec1.erase(std::remove(gameObjectvec1.begin(), gameObjectvec1.end(), pair2.second));
							gameObjectvec2.erase(std::remove(gameObjectvec2.begin(), gameObjectvec2.end(), pair1.second));
							pair1.second->OnCollissionExit(pair2.second);
							pair2.second->OnCollissionExit(pair2.second);
						}
					}
				}

			}
		}
	}
}

void Map::AddDataToSendServer()
{
	
}

void Map::UpdateClientNetData(const Json::Value& root)
{
}

void Map::UpdateServerData(const Json::Value& root)
{
}

void Map::AddPlayer(int playerID, Player& player)
{
	players.emplace_back(&player);
	levelGameObjects[playerID] = &player;
}

void Map::AddGameObjectNetDataToManagerNetData()
{
	if (!mapNetData.empty())
	{
		Json::StreamWriterBuilder writerBuilder;

		Json::Value& value = AppManager::GetAppManager()->GetNetworkManager()->GetMapNetData();
		std::string msgToSend = Json::writeString(writerBuilder, value);
		std::string goData = Json::writeString(writerBuilder, mapNetData);

		value.append(mapNetData);
		std::string postgoData = Json::writeString(writerBuilder, value);

		mapNetData.clear();
	}
}
