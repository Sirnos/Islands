#pragma once

#include "Character.hpp"

typedef std::pair<std::string, unsigned> InventoryField;

class playerCharacter :public Character
{
	InventoryField Inventory[30][30] = { std::pair<std::string,unsigned>("",0) };
	InventoryField HandInventory[6] = { std::pair<std::string,unsigned>("",0) };
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

	void move(sf::Vector2f moveVector)
	{
		CharacterShape.move(moveVector);
	}
	sf::RectangleShape *getShape()
	{
		return &CharacterShape;
	}
	void set(sf::Texture *texture, sf::Vector2f position)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(40, 60));
	}

	void setSpawnPoint(sf::Vector2f newSpawnPoint)
	{
		spawnPoint = newSpawnPoint;
	}
	sf::Vector2f getSpawnPoint()
	{
		return spawnPoint;
	}
};