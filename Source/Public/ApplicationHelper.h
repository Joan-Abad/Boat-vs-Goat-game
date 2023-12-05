#pragma once
#include <SFML/System.hpp>

class ApplicationHelper
{
public:
	static void SetDeltaTime();
	inline static float const GetDeltaTime() { return DeltaTime; };

private: 
	static float DeltaTime;
	static sf::Clock clock;
};