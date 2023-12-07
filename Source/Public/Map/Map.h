#pragma once
#include <SFML/Graphics.hpp>
#include <Player/Player.h>

class Window; 


class Map
{
public:
	Map() = default;
	virtual void InitMap(int playersQuantity) = 0;
	virtual void UpdateMap() = 0; 
	virtual void DrawWap(Window& window) = 0;
	virtual void EndMap() = 0;

protected: 
	void AddPlayer(Player player);
	std::vector<Player> players; 

private:


};
