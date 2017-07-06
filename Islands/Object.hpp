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
	}
	virtual ~Object(){}
};

struct SaplingObject : Object
{
	float PlantTime;

	SaplingObject(unsigned objectId, float AtTime)
		:Object(objectId, ObjectType::Sapling)
	{}
};

struct ChestObject : Object
{
	ItemField *Contain;

	ChestObject(unsigned objectId, size_t ChestContainSize, ItemField *objectContain = nullptr)
		:Object(objectId, ObjectType::Chest)
	{
		Contain = new ItemField[ChestContainSize];
		if (objectContain != nullptr)
		{
			for (size_t i = 0; i < ChestContainSize; i++)
			{
				Contain[i] = objectContain[i];
			}
		}
	}

	~ChestObject()
	{
		delete[] Contain;
	}
};