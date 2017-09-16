#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

#include "EntityStats.hpp"

class EntityDef
{
	EntityStats Stats;

	sf::Vector2f EntitySize;

	std::string EntityName;

public:
	EntityDef() = delete;
	EntityDef(const EntityDef &other)
		:Stats(other.Stats), EntityName(other.EntityName), EntitySize(other.EntitySize)
	{}
	EntityDef(const std::string &Name, sf::Vector2f Size, float Hp, float Mp, float Speed)
		:Stats(Hp, Mp, Speed), EntityName(Name), EntitySize(Size)
	{}
	~EntityDef() = default;


	std::string getName() { return EntityName; }
	sf::Vector2f getSize() { return EntitySize; }
	const EntityStats& getStats() { return Stats; }

};