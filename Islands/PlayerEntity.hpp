#pragma once

#include "Entity.hpp"
#include "PlayerInventory.hpp"

const float DEFAULT_PLAYER_HP = 20.0f;
const float DEFAULT_PLAYER_MP = 10.0f;
const float DEFAULT_PLAYER_SPEED = 5.0f;

class PlayerEntity :public Entity
{
	sf::Vector2f spawnPoint;
public:
	PlayerInventory Inventory;

	PlayerEntity(const sf::RectangleShape &PlayerBody, const EntityStats &PlayerStats)
		:Entity(PlayerBody, PlayerStats), Inventory()
	{}
	PlayerEntity(const sf::RectangleShape &PlayerBody, const sf::Vector2f &position, const EntityStats &PlayerStats)
		:Entity(PlayerBody, PlayerStats), Inventory()
	{
		spawnPoint = position;
		setPosition(position);
	};

	void setSpawnPoint(const sf::Vector2f &newSpawnPoint) { spawnPoint = newSpawnPoint; }
	sf::Vector2f getSpawnPoint() const { return spawnPoint; }
};