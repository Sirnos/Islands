#pragma once

#include "Character.hpp"
#include "ItemField.hpp"


class playerCharacter :public Character
{
	ItemField Inventory[PlayerFieldsNumber][PlayerFieldsNumber];
	ItemField HandInventory[PlayerFieldsNumber];
	ItemField ArmorFields[3];

	sf::Vector2f spawnPoint;
public:
	playerCharacter()
		:Character()
	{
		Inventory[0][0].assignNew(2, 20);
		Inventory[1][1].assignNew(3, 40);
		Inventory[3][3].assignNew(11, 40);

		HandInventory[1].assignNew(10, 20);
	}
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

	ItemField getInventoryField(sf::Vector2u field)
	{
		return Inventory[field.x][field.y];
	}
	ItemField getHandInventoryField(unsigned field)
	{
		return HandInventory[field];
	}
	ItemField getArmorInventoryField(unsigned field) { return ArmorFields[field]; }

	ItemField *getArmorInv() { return ArmorFields; }
	ItemField *getBelt() { return HandInventory; }
};