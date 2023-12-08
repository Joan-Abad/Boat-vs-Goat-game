#pragma once
#include <SFML/Graphics.hpp>
#include <Player/Player.h>
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
	inline std::vector<Player*>& GetPlayers() { return players;  };
protected: 
	void AddPlayer(Player& player);
	std::vector<Player*> players; 

private:


};
