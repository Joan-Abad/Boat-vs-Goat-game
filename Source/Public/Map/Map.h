#pragma once
#include <SFML/Graphics.hpp>
#include <GameObjects/Players/Player.h>
#include <SFML/Network.hpp>
class Window; 


class Map
{
public:
	Map() = default;
	~Map();
	virtual void InitMap(Window& window, int playersQuantity) = 0;
	virtual void UpdateMap(); 
	virtual void AddDataToSendServer();
	virtual void DrawWap(Window& window) = 0;
	virtual void EndMap() = 0;

	template<typename ActorToSpawn>
	ActorToSpawn* SpawnGameObject(GameObjectInitialInfo objectInitialInfo) 
	{
		ActorToSpawn* spawnedActor = new ActorToSpawn(objectInitialInfo);
		levelGameObjects.emplace_back(spawnedActor);
		return spawnedActor;
	}

	inline std::vector<Player*>& GetPlayers() { return players;  };
	inline std::vector<GameObject*>& GetGameObjects() { return levelGameObjects; };

protected: 
	void AddPlayer(Player& player);
	std::vector<Player*> players; 
	std::vector<GameObject*> levelGameObjects;
private:


};
