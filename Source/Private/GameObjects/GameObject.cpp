#include "GameObjects/GameObject.h"
#include "ApplicationHelper.h"
#include "Managers/AppManager.h"
#include "Managers/GameManager.h"
#include "Managers/Networking/NetworkingManager.h"
#include "Map/Map.h"

const char* GameObject::key_gameObjectPosition = "goPos";
const char* GameObject::key_gameObjectRot = "goRot";
const char* GameObject::key_gameObjectID = "goID";
const char* GameObject::key_gameObjectHide = "goHide";

int GameObject::gameObjectIDTracker = 0; 

GameObject::GameObject(): bTickEnabled(true), bReplicates(true), bReplicateTransform(false), forwardVector(sf::Vector2f(0.f, 1.0f)), 
bCheckCollisions(true), rightVector(sf::Vector2f(-1.f, 0.f)), objectCollision(CollisionChannels::WorldStatic), bIgnoreOwner(false), owner(nullptr)
{
	gameObjectID = gameObjectIDTracker;
	gameObjectIDTracker++;
	//std::cout << "Game Object ID: " << gameObjectID << std::endl;
}

GameObject::GameObject(GameObjectInitialInfo gameObjectInitialInfo) : GameObject()
{
	initialSprite.setPosition({ gameObjectInitialInfo.playerPosition.x,gameObjectInitialInfo.playerPosition.y });
	initialSprite.setRotation(gameObjectInitialInfo.angle);
	initialSprite.setScale(gameObjectInitialInfo.scale);
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
	window.draw(initialSprite);
}

void GameObject::Update(float deltaTime)
{

}

void GameObject::EndUpdate()
{
	if (!gameObjectNetData.empty() && bReplicates)
	{
		//AddLocalNetworkDataToSend(NetworkingManager::key_PlayerID, playerID);
		AddGameObjectNetDataToManagerNetData();
	}
}

void GameObject::UpdateClientNetData(const Json::Value& root)
{
	
}

void GameObject::UpdateServerData(const Json::Value& root)
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

void GameObject::OnCollisionEnter(GameObject* otherGO)
{
	
}

void GameObject::OnColliding(GameObject* otherGO)
{

}

void GameObject::OnCollissionExit(GameObject* otherGO)
{
}

void GameObject::AddGameObjectNetDataToManagerNetData()
{
	if (!gameObjectNetData.empty())
	{
		Json::StreamWriterBuilder writerBuilder;

		Json::Value& value = AppManager::GetNetworkManager()->GetGameObjectsNetData();
		std::string msgToSend = Json::writeString(writerBuilder, value);
		std::string goData = Json::writeString(writerBuilder, gameObjectNetData);

		value.append(gameObjectNetData);
		std::string postgoData = Json::writeString(writerBuilder, value);

		gameObjectNetData.clear();
	}
}
