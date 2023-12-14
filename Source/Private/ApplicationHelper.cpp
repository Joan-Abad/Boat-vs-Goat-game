#include "ApplicationHelper.h"
#include <iostream>

float ApplicationHelper::DeltaTime;
sf::Clock ApplicationHelper::clock;

void ApplicationHelper::SetDeltaTime()
{
	DeltaTime = clock.restart().asSeconds();
}

sf::Vector2f ApplicationHelper::Normalize(const sf::Vector2f& vector)
{
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);

    if (length != 0) {
        return sf::Vector2f(vector.x / length, vector.y / length);
    }
    else {
        // Handle division by zero or zero-length vectors
        std::cerr << "Warning: Attempt to normalize a zero-length vector." << std::endl;
        return sf::Vector2f(0.f, 0.f);
    }
}

sf::Vector2f ApplicationHelper::rotateVector(const sf::Vector2f& vector, float angleDegrees)
{
    sf::Transform transform;
    transform.rotate(angleDegrees);

    return transform.transformPoint(vector);
}

void ApplicationHelper::RestartClock()
{
    clock.restart().asSeconds();
}

float ApplicationHelper::AngleBetweenTwoVectors(const sf::Vector2f& a, const sf::Vector2f& b)
{
    // Normalize the vectors
    sf::Vector2f normalizedA = a / std::sqrt(a.x * a.x + a.y * a.y);
    sf::Vector2f normalizedB = b / std::sqrt(b.x * b.x + b.y * b.y);

    // Calculate the dot product
    float dotProduct = normalizedA.x * normalizedB.x + normalizedA.y * normalizedB.y;

    // Use arccos to find the angle in radians
    float angleRadians = std::acos(dotProduct);

    // Convert radians to degrees if needed
    float angleDegrees = angleRadians * 180.0f / 3.14159265f;

    return angleDegrees;
}
