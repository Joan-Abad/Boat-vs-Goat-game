#pragma once
#include "Map/Map.h"
#include <SFML/Graphics.hpp>
#include <string>

class Map_Lake : public Map
{
public:
	Map_Lake();
	~Map_Lake();
	virtual void InitMap(Window& window, int playersQuantity) override;
	virtual void UpdateMap() override;
	virtual void DrawWap(Window& window) override;
	virtual void EndMap() override;

	const char* backgroundLakePath = "Art/Background/Water.jpg";
	const char* boat1Path = "Art/boat/PNG/Boats_color1/Boat_color1_1.png";
	const char* boat2Path = "Art/boat/PNG/Boats_color3/Boat_color3_1.png";
	const char* bulletPath = "Art/bullet.png";
private: 
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite; 

};