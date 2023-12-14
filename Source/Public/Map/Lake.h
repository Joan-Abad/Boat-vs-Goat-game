#pragma once
#include "Map/Map.h"
#include <SFML/Graphics.hpp>
#include <string>

#define MaxMissilsOnScreen 10

class Cannon; 
class Bullet; 
class Map_Lake : public Map
{
public:
	Map_Lake();
	~Map_Lake();
	virtual void InitMap(Window& window, int playersQuantity) override;
	virtual void UpdateMap(float DeltaTime) override;
	virtual void DrawWap(Window& window) override;
	//Call it from the server always
	virtual bool CheckWinCondition() override;
	virtual void EndMap() override;

	//Recieved data from the server. Extract any information needed
	virtual void UpdateClientNetData(const Json::Value& root);

	//Textures
	const char* backgroundLakePath = "Art/Background/Water.jpg";
	const char* boat1Path = "Art/PirateBoats/RedBoat.png";
	const char* boat2Path = "Art/PirateBoats/BlueBoat.png";
	const char* boat3Path = "Art/PirateBoats/GreenBoat.png";
	const char* boat4Path = "Art/PirateBoats/YellowBoat.png";
	const char* bulletPath = "Art/bullet.png";
	const char* heartPath = "Art/heart.png";
	const char* goatPath = "Art/goat.png";
	const char* missilePath = "Art/cannon.png";
	const char* sharkPath = "Art/shark.png";

	//Fonts
	const char* lakeFontPath = "Fonts/Mikeona.ttf";
	
	//Replication keys
	const char* key_win = "winCon";
	const char* key_SpawnMissile = "spMI";
	const char* key_SpawnPos = "spPo";
	const char* key_SpawnRot = "spRo";

	bool gameOver; 
	sf::Sprite goatSprite;

private: 
	void SpawnMapCannons();
	//Missile
	void SpawnMissile();
	void SetNewSpawnTimeMissle();

	void FinishMap(const char* playerName);
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite; 
	sf::Text winningText;
	// Create a clock
	sf::Clock timer;
	int totalCannons;
	float finalSpawnTimeMissile; 
	float spawnTimeMissileMin;
	float spawnTimeMissileMax; 
	int missileTracker; 
	std::vector<Cannon*> cannons; 
	std::array<Bullet*, MaxMissilsOnScreen> missiles;
};