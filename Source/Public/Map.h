#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
	Map(const char* backgroundPath);
	void DrawWap(sf::RenderWindow& window);
private:
	sf::Sprite backgroundSprite;
	sf::Texture texture;
};
