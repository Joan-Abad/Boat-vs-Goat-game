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
	void EndUpdate();
	void CheckCollisions();
	virtual void AddDataToSendServer();
	virtual bool CheckWinCondition() = 0; 
	virtual void DrawWap(Window& window) = 0;
	virtual void EndMap() = 0;

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root);

	//Recived data from a client. Extract any information needed
	virtual void UpdateServerData(const Json::Value& root);

	template<typename ActorToSpawn, typename... Args>
	ActorToSpawn* SpawnGameObject(Args&&... args) 
	{
		ActorToSpawn* spawnedActor = new ActorToSpawn(std::forward<Args>(args)...);
		levelGameObjects[spawnedActor->GetGameObjectID()] = spawnedActor;
		return spawnedActor;
	}

	//Adds the data that will be send at the end of the frame
	//Value needs to be a supported JSON type (string, number, Json Object, array, bool or null)
	template <typename Value>
	void AddLocalNetworkDataToSend(const char* KEY, Value valueToSend)
	{
		mapNetData[KEY] = valueToSend;
	}


	inline std::vector<Player*>& GetPlayers() { return players;  };
	inline std::unordered_map<int, GameObject*>& GetGameObjects() { return levelGameObjects; };

protected: 
	void AddPlayer(int playerID, Player& player);
	std::vector<Player*> players; 
	//int = game object ID
	std::unordered_map<int, GameObject*> levelGameObjects;

	//Map net data transmitter over the network
	Json::Value mapNetData;

	//Passes the game object network data to the NetworkManager
	void AddGameObjectNetDataToManagerNetData();
private:


};
