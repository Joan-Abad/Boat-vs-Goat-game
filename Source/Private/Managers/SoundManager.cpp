#include "Managers/SoundManager.h"
#include <iostream>

SoundManager* SoundManager::instance;


Sound::Sound(const char* soundPath)
{
	if (!soundBuffer.loadFromFile(soundPath))
	{
		std::cerr << " Failed to load sound buffer file: " << soundPath;
		return;
	}

	sound.setBuffer(soundBuffer);
}

void Sound::SetVolume(float soundVolume)
{
	sound.setVolume(soundVolume);
}

void Sound::PlaySound(bool loop)
{
	sound.play();
	sound.setLoop(loop);
}

void Sound::StopSound()
{
	sound.stop();
}

Sound* SoundManager::CreateSound(const char* soundPath)
{
	Sound* newSound = new Sound(soundPath);
	
	if (newSound)
		gameSounds[soundPath] = newSound; 

	return newSound;
}

Sound* SoundManager::GetSound(const char* soundPath)
{
	return gameSounds[soundPath];
}

bool SoundManager::RemoveSound(const char* soundPath)
{
	bool removedTexture = gameSounds.erase(soundPath) != 0;

	if (removedTexture)
		std::cout << "Removed texture " << soundPath << " correctly!\n";
	else
		std::cerr << "Tried to remove a texture with this path: " << soundPath << " that did not exist.";

	return removedTexture;

}

SoundManager::~SoundManager()
{
	for (auto& gameSound : gameSounds)
		delete gameSound.second;
}
