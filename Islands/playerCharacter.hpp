#pragma once

#include "Character.hpp"
#include "ItemField.hpp"


class playerCharacter :public Character
{
	ItemField Inventory[PlayerFieldsNumber][PlayerFieldsNumber];
	ItemField HandInventory[6];
	ItemField ArmorFields[3];

	sf::Vector2f spawnPoint;
public:
	playerCharacter()
		:Character()
	{}
	playerCharacter(sf::Texture *texture, sf::Vector2f position,sf::Vector2f stats)
		:Character(stats)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(10, 10));
	}

	void move(sf::Vector2f moveVector) { CharacterShape.move(moveVector); }
	sf::RectangleShape *getShape() { return &CharacterShape; }
	void set(sf::Texture *texture, sf::Vector2f position,sf::Vector2f stats)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(40, 60));
		HP = stats.x;
		MP = stats.y;
	}

	void setSpawnPoint(sf::Vector2f newSpawnPoint) { spawnPoint = newSpawnPoint; }
	sf::Vector2f getSpawnPoint() { return spawnPoint; }

	ItemField getInventoryField(sf::Vector2u fieldNumber)
	{
		return Inventory[fieldNumber.x][fieldNumber.y];
	}
	ItemField getHandInventoryField(unsigned fieldNumber)
	{
		return HandInventory[fieldNumber];
	}
};