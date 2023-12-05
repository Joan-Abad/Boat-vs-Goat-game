#include "ApplicationHelper.h"

float ApplicationHelper::DeltaTime;
sf::Clock ApplicationHelper::clock;

void ApplicationHelper::SetDeltaTime()
{
	DeltaTime = clock.restart().asSeconds();
}
