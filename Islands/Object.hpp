#pragma once

#include "ObjectDef.hpp"
#include "ItemField.hpp"
#include <vector>

class Object
{
	ObjectType type;
	unsigned Id;

	bool Collision;

public:
	Object(unsigned objectId, bool objectCollision, ObjectType objectType = ObjectType::Default)
		:type(objectType), Id(objectId), Collision(objectCollision)
	{}
	virtual ~Object() = default;


	ObjectType getType() const
	{
		return type;
	}
	unsigned getId() const
	{
		return Id;
	}
	bool isCollidable() const
	{
		return Collision;
	}

};

class SaplingObject : public Object
{
	float PlantTime;

public:
	SaplingObject(unsigned objectId, float AtTime)
		:Object(objectId, false, ObjectType::Sapling), PlantTime(AtTime)
	{}
	~SaplingObject() = default;


	float getPlantTime() const
	{
		return PlantTime;
	}

};

class ChestObject : public Object
{
	std::vector<ItemField> Contain;

public:
	ChestObject(unsigned objectId, size_t ChestSize)
		:Object(objectId, true, ObjectType::Chest), Contain(ChestSize)
	{}
	ChestObject(unsigned objectId, const std::vector<ItemField> &itemsInContainer)
		:Object(objectId, true, ObjectType::Chest), Contain(itemsInContainer)
	{}
	~ChestObject()
	{
		Contain.clear();
	}


	std::vector<ItemField> &getContain()
	{
		return Contain;
	}

};

class CraftingPlaceObject : public Object
{
	std::vector<Recipe> Recipes;

public:
	CraftingPlaceObject(unsigned objectId, const std::vector<Recipe> &ObjectRecipes)
		:Object(objectId, true, ObjectType::CraftingPlace), Recipes(ObjectRecipes)
	{
		Recipes.shrink_to_fit();
	}
	~CraftingPlaceObject() = default;


	const std::vector<Recipe> &getRecipes() const
	{
		return Recipes;
	}

};