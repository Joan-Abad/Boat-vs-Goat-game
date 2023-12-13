#pragma once
#include <SFML/System.hpp>
#include "SFML/Graphics.hpp"
#include <random>

class ApplicationHelper
{
public:
	static void SetDeltaTime();
	
	//Math helper functions
	static sf::Vector2f Normalize(const sf::Vector2f& vector);
	static sf::Vector2f rotateVector(const sf::Vector2f& vector, float angleDegrees);
	template <typename type>
	static type GetRandomValue(type min, type max)
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<type> distribution(min, max + 1);
		return distribution(rd);
	}
	inline static float const GetDeltaTime() { return DeltaTime; };

private: 
	static float DeltaTime;
	static sf::Clock clock;
};