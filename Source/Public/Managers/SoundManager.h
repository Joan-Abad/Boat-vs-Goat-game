#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>

#define SOUND_ENABLED 1

class Sound
{
	friend class SoundManager; 
public:
	Sound(const char* soundPath);
	void SetVolume(float soundVolume);
	void PlaySound(bool loop = false);
	void StopSound();
private:
	sf::SoundBuffer soundBuffer;

	sf::Sound sound;
};

class SoundManager
{
public:
	static SoundManager* Get()
	{
		if (!instance)
			instance = new SoundManager();

		return instance;
	}

	//Returns the sound if could be created. If not nullptr will be returned
	Sound* CreateSound(const char* soundPath);
	bool RemoveSound(const char* soundPath);
	Sound* GetSound(const char* soundPath);

private:
	SoundManager() = default;
	~SoundManager();

	std::unordered_map<const char*, Sound*> gameSounds;

	static SoundManager* instance;

};