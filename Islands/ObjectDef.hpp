#pragma once

#include "Recipe.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <string>

typedef std::pair<std::string, unsigned> Yield;

enum class ObjectType
{
	Default,
	Chest,
	CraftingPlace,
	Tree,
	Sapling,
	Spawner
};

class ObjectDef
{
	ObjectType type;

	std::string ObjName;
	sf::Vector2i ObjSize;
	sf::FloatRect ObjCollisionBox;
	bool ObjDestructible;
	Yield ObjYield;
public:
	ObjectType getType() { return type; }
	std::string getName() { return ObjName; }
	sf::Vector2i getSize() { return ObjSize; }
	sf::FloatRect getCollisionBox() { return ObjCollisionBox; }
	bool getDestructible() { return ObjDestructible; }
	Yield getYield() { return ObjYield; }

	ObjectDef(std::string Name, sf::Vector2i Size, sf::FloatRect CollisionBox, Yield yield, bool Destructible,ObjectType type = ObjectType::Default)
		:ObjDestructible(Destructible),
		ObjName(Name),
		ObjSize(Size),
		ObjCollisionBox(CollisionBox),
		ObjYield(yield),
		type(type)
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

	ChestDef(std::string Name, sf::Vector2i Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, unsigned ChestCapacity)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible,ObjectType::Chest)
	{
		Capacity = ChestCapacity;
	}
};

class CraftingPlaceDef : public ObjectDef
{
	std::vector<RecipeDef> Recipes;

public:
	std::vector<RecipeDef> & getRecipes() { return Recipes; }

	CraftingPlaceDef(std::string Name, sf::Vector2i Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, std::vector<RecipeDef> RecipeVect)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible,ObjectType::CraftingPlace)
	{
		Recipes = RecipeVect;
	}
};

class SaplingDef : public ObjectDef
{
	float GrowTime;
	std::string GrowTo;

public:
	float getGrowTime() { return GrowTime; }
	std::string getGrowTo() { return GrowTo; }

	SaplingDef(std::string Name, sf::Vector2i Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, float TimeForGrow,std::string For)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible,ObjectType::Sapling)
	{
		GrowTo = For;
		GrowTime = TimeForGrow;
	}
};

class SpawnerDef : public ObjectDef
{
	float SpawnTime;
	std::string MonsterName;
public:
	float getSpawnTime() { return SpawnTime; }
	std::string getMonsterName() { return MonsterName; }

	SpawnerDef(std::string Name, sf::Vector2i Size, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible, float TimeForSpawn, std::string MonsterToSpawn)
		:ObjectDef(Name, Size, CollisionBox, yield, Destructible,ObjectType::Spawner)
	{
		SpawnTime = TimeForSpawn;
		MonsterName = MonsterToSpawn;
	}
};