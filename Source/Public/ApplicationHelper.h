#pragma once
#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"

class ApplicationHelper
{
public:
	static void SetDeltaTime();
	
	//Math helper functions
	static sf::Vector2f Normalize(const sf::Vector2f& vector);
	static sf::Vector2f rotateVector(const sf::Vector2f& vector, float angleDegrees);

	inline static float const GetDeltaTime() { return DeltaTime; };

private: 
	static float DeltaTime;
	static sf::Clock clock;
};