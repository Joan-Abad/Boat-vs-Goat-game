#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class FontManager
{
public: 
	static FontManager* Get()
	{
		if (!instance)
			instance = new FontManager();

		return instance; 
	}

private:
	FontManager() = default;
	~FontManager();
	std::unordered_map<const char*, sf::Font*> gameFonts;

	static FontManager* instance;
public: 

	//Returns if could add the texture to the manager
	bool AddFont(const char* fontPath);
	bool HasFont(const char* fontPath);
	sf::Font* GetFont(const char* fontPath);
	bool RemoveFont(const char* fontPath);
};