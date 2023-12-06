#pragma once
#include <SFML/Graphics.hpp>

class Window; 

class Map
{
public:
	Map(const char* backgroundPath);
	void DrawWap(Window& window);
private:
	sf::Sprite backgroundSprite;
	sf::Texture texture;
};
