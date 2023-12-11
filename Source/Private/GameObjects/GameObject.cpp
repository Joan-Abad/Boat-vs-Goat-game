#include "GameObjects/GameObject.h"
#include "ApplicationHelper.h"

GameObject::GameObject(GameObjectInitialInfo gameObjectInitialInfo) : bReplicates(false), bReplicateTransform(false),
	forwardVector(sf::Vector2f(0.f, 1.0f)), rightVector(sf::Vector2f(-1.f, 0.f))
{
}

void GameObject::Init()
{
	
}

void GameObject::Draw(sf::RenderWindow& window)
{

}

void GameObject::Update()
{

}

void GameObject::EndUpdate()
{

}

void GameObject::SetGameObjectTransform(sf::Vector2f position, float angle, sf::Vector2f scale)
{
	SetPosition(position);
	SetRotation(angle);
	SetScale(scale);
}

Transform GameObject::GetActorTransform()
{
	return Transform(GetPosition(), GetRotation(), GetScale());
}

void GameObject::SetPosition(sf::Vector2f newPosition)
{
	initialSprite.setPosition(newPosition);
	Json::Value floatArray;
	floatArray.append(newPosition.x);
	floatArray.append(newPosition.y);

	//root["playerPosition"] = floatArray;
}

void GameObject::SetRotation(float angle)
{
	forwardVector = ApplicationHelper::rotateVector(sf::Vector2f(0.0f, -1.0f), angle);
	rightVector = ApplicationHelper::rotateVector(sf::Vector2f(-1.0f, 0.0f), angle);
	initialSprite.setRotation(angle);
	//root["playerAngle"] = angle;
}

float GameObject::GetRotation()
{
	return initialSprite.getRotation();
}

void GameObject::SetScale(sf::Vector2f newScale)
{
	return initialSprite.setScale(newScale);
}

sf::Vector2f GameObject::GetScale()
{
	return initialSprite.getScale();
}

sf::Vector2f GameObject::GetPosition()
{
	return initialSprite.getPosition();
}
