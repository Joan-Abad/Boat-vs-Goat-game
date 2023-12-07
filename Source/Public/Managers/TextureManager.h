#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>

class TextureManager
{
public: 
	static TextureManager& GetTextureManager()
	{
		static TextureManager textManager;
		return textManager;
	}
private: 
	TextureManager() = default;
	std::unordered_map<const char*, sf::Texture*> gameTextures;

public: 
	//Returns if could add the texture to the manager
	bool AddTexture(const char* texturePath);
	bool HasTexture(const char* texturePath);
	sf::Texture* GetTexture(const char* texturePath);
	bool RemoveTexture(const char* texturePath);
};