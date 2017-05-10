#pragma once

#include <vector>
#include <string>

#include "ObjectDef.hpp"
#include "Log.hpp"

class ObjectArray
{
	std::vector<ObjectDef> ObjectVector;
public:
	~ObjectArray() { ObjectVector.clear(); }

	void generateArray(std::vector<ObjectDef> &Other) { ObjectVector = Other; }

	bool isObjectExist(std::string OtherObjName)
	{
		for (auto & i : ObjectVector)
		{
			if (i.getName() == OtherObjName)
			{
				return true;
			}
		}

		return false;
	}
	ObjectDef* getObject(std::string ObjectName)
	{
		for (auto i : ObjectVector)
		{
			if (i.getName() == ObjectName)
			{
				return &i;
			}
		}
		return nullptr;
	}
	ObjectDef* getObject(size_t Pos) { return &ObjectVector[Pos]; }

	const std::vector<ObjectDef> &getObjects() { return ObjectVector; }
};