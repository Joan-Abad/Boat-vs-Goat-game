#include "Managers/TextureManager.h"
#include <iostream>

bool TextureManager::AddTexture(const char* texturePath)
{
	if (!HasTexture(texturePath))
	{
		sf::Texture* texture = new sf::Texture();
		if (texture)
		{
			if (!texture->loadFromFile(texturePath))
			{
				std::cerr << "Could not load the following texture: " << texturePath << std::endl;
				delete texture;
				return false;
			}

			gameTextures[texturePath] = texture;
			//std::cout << "Texture " << texturePath << " created and added to the Texture Manager\n";
			return true;
		}
		else
		{
			std::cerr << "Texture failed to create. Check " << __LINE__ << " in file " << __FILE__ << std::endl;
			return false; 
		}
	}
	std::cout << "Trying to create an already existing texture: " << texturePath << std::endl;
	return false;
}

bool TextureManager::HasTexture(const char* texturePath)
{
	return gameTextures[texturePath] != nullptr;
}

sf::Texture* TextureManager::GetTexture(const char* texturePath)
{
	if (!HasTexture(texturePath))
	{
		bool textureAdded = AddTexture(texturePath);

		if (textureAdded)
			return gameTextures[texturePath];
		else
			return nullptr; 
	}
	return gameTextures[texturePath];
}

bool TextureManager::RemoveTexture(const char* texturePath)
{
	bool removedTexture = gameTextures.erase(texturePath) != 0;
	
	if (removedTexture)
		std::cout << "Removed texture " << texturePath << " correctly!\n";
	else
		std::cerr << "Tried to remove a texture with this path: " << texturePath << " that did not exist.";

	return removedTexture;
}
