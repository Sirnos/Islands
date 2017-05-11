#pragma once

#include "Character.hpp"

//first = ID of item,second = amount
typedef std::pair<unsigned, unsigned> InventoryField;

class playerCharacter :public Character
{
	InventoryField Inventory[21][21] = { InventoryField(0,0) };
	InventoryField HandInventory[6] = { InventoryField(0,0) };
	sf::Vector2f spawnPoint;
public:
	playerCharacter()
	{
		HP = 10;
		MP = 6;
	}
	playerCharacter(sf::Texture *texture, sf::Vector2f position)
		:playerCharacter()
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(10, 10));
	}

	void move(sf::Vector2f moveVector) { CharacterShape.move(moveVector); }
	sf::RectangleShape *getShape() { return &CharacterShape; }
	void set(sf::Texture *texture, sf::Vector2f position)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(40, 60));
	}

	void setSpawnPoint(sf::Vector2f newSpawnPoint) { spawnPoint = newSpawnPoint; }
	sf::Vector2f getSpawnPoint() { return spawnPoint; }

	InventoryField getInventoryField(sf::Vector2u fieldNumber)
	{
		return Inventory[fieldNumber.x][fieldNumber.y];
	}
	InventoryField getHandInventoryField(unsigned fieldNumber)
	{
		return HandInventory[fieldNumber];
	}
};