#pragma once
#include <SFML/Graphics.hpp>
#include <GameObjects/Players/Player.h>
#include <SFML/Network.hpp>
#include <unordered_map>

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

	template<typename ActorToSpawn, typename... Args>
	ActorToSpawn* SpawnGameObject(Args&&... args) 
	{
		ActorToSpawn* spawnedActor = new ActorToSpawn(std::forward<Args>(args)...);
		levelGameObjects[spawnedActor->GetGameObjectID()] = spawnedActor;
		return spawnedActor;
	}

	inline std::vector<Player*>& GetPlayers() { return players;  };
	inline std::unordered_map<int, GameObject*>& GetGameObjects() { return levelGameObjects; };

protected: 
	void AddPlayer(int playerID, Player& player);
	std::vector<Player*> players; 
	//int = game object ID
	std::unordered_map<int, GameObject*> levelGameObjects;
private:


};
