#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

#include "EntityStats.hpp"
#include "EntityBehavior.hpp"

class EntityDef
{
protected:
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
	EntityDef(const std::string &Name, const EntityStats &stats, sf::Vector2f size)
		:EntityName(Name), Stats(stats), EntitySize(size)
	{}
	~EntityDef() = default;
	EntityDef& operator=(const EntityDef& other) = delete;

	std::string getName() const { return EntityName; }
	sf::Vector2f getSize() const { return EntitySize; }
	const EntityStats& getStats() const { return Stats; }

};

class MonsterEntityDef : public EntityDef
{
	BehaviorVariables Behavior;

public:
	MonsterEntityDef() = delete;
	MonsterEntityDef(const MonsterEntityDef &other)
		:EntityDef(other.EntityName, other.Stats, other.EntitySize), Behavior(other.Behavior)
	{}
	MonsterEntityDef(const std::string &name, const EntityStats &stats, const BehaviorVariables &behavior, sf::Vector2f size)
		:EntityDef(name, stats, size), Behavior(behavior)
	{}
	MonsterEntityDef& operator=(const MonsterEntityDef& other) = delete;

	BehaviorVariables getBehavior() const { return Behavior; }
};