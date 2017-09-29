#pragma once

#include "ObjectDef.hpp"
#include "ItemField.hpp"
#include <vector>

class Object
{
	ObjectType type;
	unsigned Id;

public:
	Object(unsigned objectId, ObjectType objectType = ObjectType::Default)
		:type(objectType), Id(objectId)
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
};

class SaplingObject : public Object
{
	float PlantTime;

public:
	SaplingObject(unsigned objectId, float AtTime)
		:Object(objectId, ObjectType::Sapling), PlantTime(AtTime)
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
		:Object(objectId, ObjectType::Chest), Contain(ChestSize)
	{}
	ChestObject(unsigned objectId, const std::vector<ItemField> &otherContainer)
		:Object(objectId, ObjectType::Chest), Contain(otherContainer)
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
		:Object(objectId, ObjectType::CraftingPlace), Recipes(ObjectRecipes)
	{
		Recipes.shrink_to_fit();
	}
	~CraftingPlaceObject() = default;

	const std::vector<Recipe> &getRecipes() const
	{
		return Recipes;
	}
};