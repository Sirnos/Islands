#pragma once

#include "Entity.hpp"
#include "PlayerInventory.hpp"


class PlayerEntity :public Entity
{
	sf::Vector2f spawnPoint;
public:
	PlayerInventory Inventory;

	PlayerEntity(const sf::RectangleShape &PlayerBody, sf::Vector2f position,float HP,float MP,float Speed)
		:Entity(PlayerBody,HP,MP,Speed)
	{}

	void move(sf::Vector2f Vector) { Body.move(Vector); }

	void setSpawnPoint(sf::Vector2f newSpawnPoint) { spawnPoint = newSpawnPoint; }
	sf::Vector2f getSpawnPoint() { return spawnPoint; }

};