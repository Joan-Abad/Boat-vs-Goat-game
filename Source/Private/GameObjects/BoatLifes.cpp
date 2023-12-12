#include "GameObjects/BoatLifes.h"
#include "Managers/TextureManager.h"
#include "Managers/GameManager.h"
#include "GameObjects/Players/Boat.h"
#include "Managers/FontManager.h"

#define HEARTPATH "Art/heart.png"

BoatLifes::BoatLifes(GameObjectInitialInfo initialInfo, Boat* associatedBoat) : GameObject(initialInfo), fontSize(12.f)
{
	this->associatedBoat = associatedBoat; 
	int boatCounter = associatedBoat->boatCounter;
	float scale = 0.5f; 
	
	heart.setTexture(*TextureManager::GetTextureManager().GetTexture(HEARTPATH));

	sf::Font* font =  FontManager::Get()->GetFont("Fonts/Mikeona.ttf");
	lifeText.setFont(*font);
	lifeText.setString("3");
	lifeText.setCharacterSize(32.f);
	int numOffset = 32.f/2;
	if (boatCounter == 0)
	{
		initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture(PLAYER1TEXTPATH));
		initialSprite.setScale(sf::Vector2f({ scale , scale }));
		SetRotation(0.f);
		heart.setPosition(initialSprite.getTexture()->getSize().x * scale, 0);
		lifeText.setPosition(initialSprite.getTexture()->getSize().x * scale + numOffset * 1.5f, numOffset/2);
		//sf::Vector2f vec = sf::Vector2f(initialSprite.getTexture()->getSize().x * scale, initialSprite.getTexture()->getSize().y * scale);
		//initialSprite.setPosition(vec);

	}
	else if (boatCounter == 1)
	{
		initialSprite.setTexture(*TextureManager::GetTextureManager().GetTexture(PLAYER2TEXTPATH));
		initialSprite.setScale(sf::Vector2f({ scale , scale }));
		//SetRotation(0.f);
		float spriteOffset = initialSprite.getTexture()->getSize().x * scale;
		float spritePos = WINDOW_SIZE.x;
		initialSprite.setPosition(spritePos - spriteOffset * 2, 0);
		heart.setPosition(spritePos - spriteOffset, 0);
		lifeText.setPosition(spritePos - spriteOffset + numOffset * 1.5f, numOffset/2);

	}

	heart.setScale(0.08f, 0.08f);
}

void BoatLifes::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);
	window.draw(heart);
	window.draw(lifeText);
}
