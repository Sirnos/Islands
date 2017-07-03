#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>


typedef std::pair<std::string, unsigned> Yield;

enum class ObjectType
{
	Default,
	Chest,
	Tree,
	Sapling,
	Spawner
};

class ObjectDef
{
protected:
	ObjectType type;

private:
	std::string ObjName;
	sf::Vector2f ObjSize;
	sf::FloatRect ObjCollisionBox;
	bool ObjDestructible;
	Yield ObjYield;
public:
	ObjectType getType() { return type; }
	std::string getName() { return ObjName; }
	sf::Vector2f getSize() { return ObjSize; }
	sf::FloatRect getCollisionBox() { return ObjCollisionBox; }
	bool getDestructible() { return ObjDestructible; }
	Yield getYield() { return ObjYield; }

	ObjectDef(std::string Name, sf::Vector2f Size, sf::FloatRect CollisionBox, Yield yield, bool Destructible)
		:ObjDestructible(Destructible),
		ObjName(Name),
		ObjSize(Size),
		ObjCollisionBox(CollisionBox),
		ObjYield(yield),
		type(ObjectType::Default)
	{}
	ObjectDef()
		:ObjDestructible(false),
		ObjName(""),
		ObjSize(0,0),
		ObjCollisionBox(0,0,0,0),
		ObjYield(Yield("",0)),
		type(ObjectType::Default)
	{}

	virtual ~ObjectDef(){}
};

class ChestDef : public ObjectDef
{
	unsigned Capacity;

public:
	unsigned getCapacity() { return Capacity; }

	ChestDef(std::string Name, sf::Vector2f Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, unsigned ChestCapacity)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible)
	{
		Capacity = ChestCapacity;
		type = ObjectType::Chest;
	}
};

class SaplingDef : public ObjectDef
{
	float GrowTime;

public:
	float getGrowTime() { return GrowTime; }

	SaplingDef(std::string Name, sf::Vector2f Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, float TimeForGrow)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible)
	{
		GrowTime = TimeForGrow;
		type = ObjectType::Sapling;
	}
};

class SpawnerDef : public ObjectDef
{
	float SpawnTime;
	std::string MonsterName;
public:
	float getSpawnTime() { return SpawnTime; }
	std::string getMonsterName() { return MonsterName; }

	SpawnerDef(std::string Name, sf::Vector2f Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, float TimeForSpawn, std::string MonsterToSpawn)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible)
	{
		SpawnTime = TimeForSpawn;
		MonsterName = MonsterToSpawn;
		type = ObjectType::Spawner;
	}
};