#pragma once
#include "Map/Map.h"
#include <SFML/Graphics.hpp>
#include <string>

#define MaxMissilsOnScreen 10

class Missile; 

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
	const char* boat1Path = "Art/boat/PNG/Boats_color1/Boat_color1_1.png";
	const char* boat2Path = "Art/boat/PNG/Boats_color3/Boat_color3_1.png";
	const char* bulletPath = "Art/bullet.png";
	const char* heartPath = "Art/heart.png";
	const char* goatPath = "Art/goat.png";
	const char* missilePath = "Art/missile.png";

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
	//Missile
	void SpawnMissile();
	void SetNewSpawnTimeMissle();

	void FinishMap(const char* playerName);
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite; 
	sf::Text winningText;
	// Create a clock
	sf::Clock timer;
	float finalSpawnTimeMissile; 
	float spawnTimeMissileMin;
	float spawnTimeMissileMax; 
	int missileTracker; 
	std::array<Missile*, MaxMissilsOnScreen> missiles;
};