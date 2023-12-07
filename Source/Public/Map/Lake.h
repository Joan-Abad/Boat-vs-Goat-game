#pragma once
#include "Map/Map.h"
#include <SFML/Graphics.hpp>
#include <string>

class Map_Lake : public Map
{
public:
	Map_Lake();

	virtual void InitMap(int playersQuantity) override;
	virtual void UpdateMap() override;
	virtual void DrawWap(Window& window) override;
	virtual void EndMap() override;

private: 
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite; 

	const std::string backgroundLakePath = "Art/Background/Water.jpg";
};