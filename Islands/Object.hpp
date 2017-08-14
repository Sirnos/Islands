#pragma once

#include "ObjectDef.hpp"
#include "ItemField.hpp"
#include <vector>

struct Object
{
	ObjectType type;
	unsigned Id;

	Object(unsigned objectId, ObjectType objectType = ObjectType::Default)
	{
		Id = objectId;
		type = objectType;
	}
	virtual ~Object(){}
};

struct SaplingObject : Object
{
	float PlantTime;

	SaplingObject(unsigned objectId, float AtTime)
		:Object(objectId, ObjectType::Sapling)
	{
		PlantTime = AtTime;
	}
};

struct ChestObject : Object
{
	std::vector<ItemField> Contain;

	ChestObject(unsigned objectId, size_t ChestSize)
		:Object(objectId, ObjectType::Chest)
	{
		Contain.resize(ChestSize);
	}
	ChestObject(unsigned objectId, std::vector<ItemField> &otherContainer)
		:Object(objectId, ObjectType::Chest)
	{
		Contain = otherContainer;
	}

	~ChestObject()
	{
		Contain.clear();
	}
};

struct CraftingPlaceObject : Object
{
	std::vector<Recipe> Recipes;
	CraftingPlaceObject(unsigned objectId, std::vector<Recipe> ObjectRecipes)
		:Object(objectId, ObjectType::CraftingPlace)
	{
		Recipes = ObjectRecipes;
	}
};