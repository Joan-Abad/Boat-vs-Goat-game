#include "GameObjects/GameObject.h"
#include "ApplicationHelper.h"
#include "Managers/AppManager.h"
#include "Managers/GameManager.h"
#include "Map/Map.h"

GameObject::GameObject(): bTickEnabled(true)
{
	initialSprite.setPosition({ 0,0 });
	initialSprite.setRotation(0);
}

GameObject::GameObject(GameObjectInitialInfo gameObjectInitialInfo) : bReplicates(false), bReplicateTransform(false),
	forwardVector(sf::Vector2f(0.f, 1.0f)), rightVector(sf::Vector2f(-1.f, 0.f)), bTickEnabled(true)
{
}

void GameObject::Init()
{
	
}

void GameObject::DestroyActor()
{
	//GetCurrentMap()->GetGameObjects().()
}

void GameObject::ShowGameObject()
{
	initialSprite.setColor(sf::Color(255.0, 255.0, 255.0, 255.0));
}

void GameObject::HideGameObject()
{
	initialSprite.setColor(sf::Color::Transparent);
}

bool GameObject::GameObjectOutOfScreen()
{
	if (initialSprite.getPosition().x < 0 || initialSprite.getPosition().y < 0
		|| initialSprite.getPosition().x > WINDOW_SIZE.x || initialSprite.getPosition().y > WINDOW_SIZE.y)
	{
		std::cout << "Game object is out of screen";
		return true; 
	}
	return false; 
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

Map* GameObject::GetCurrentMap()
{
	return GameManager::GetGameManager()->GetCurrentMap();
}
