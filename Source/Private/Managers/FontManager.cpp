#include "Managers/FontManager.h"
#include <iostream>

FontManager* FontManager::instance = nullptr;


bool FontManager::AddFont(const char* fontPath)
{
	if (!HasFont(fontPath))
	{
		sf::Font* font = new sf::Font();
		if (font)
		{
			if (!font->loadFromFile(fontPath))
			{
				std::cerr << "Could not load the following texture: " << fontPath << std::endl;
				delete font;
				return false;
			}

			gameFonts[fontPath] = font;
			std::cout << "Texture " << fontPath << " created and added to the Texture Manager\n";
			return true;
		}
		else
		{
			std::cerr << "Texture failed to create. Check " << __LINE__ << " in file " << __FILE__ << std::endl;
			return false;
		}
	}
	std::cout << "Trying to create an already existing texture: " << fontPath << std::endl;
	return false;
}

bool FontManager::HasFont(const char* fontPath)
{
	return gameFonts[fontPath] != nullptr;
}

sf::Font* FontManager::GetFont(const char* fontPath)
{
	if (!HasFont(fontPath))
	{
		bool fontAdded = AddFont(fontPath);

		if (fontAdded)
			return gameFonts[fontPath];
		else
			return nullptr;
	}
	return gameFonts[fontPath];
}

bool FontManager::RemoveFont(const char* fontPath)
{
	bool removedFont = gameFonts.erase(fontPath) != 0;

	if (removedFont)
		std::cout << "Removed texture " << fontPath << " correctly!\n";
	else
		std::cerr << "Tried to remove a texture with this path: " << fontPath << " that did not exist.";

	return removedFont;
}
