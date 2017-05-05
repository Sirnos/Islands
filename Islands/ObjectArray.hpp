#pragma once

#include <vector>
#include <string>

#include "object.hpp"
#include "Log.hpp"

class ObjectArray
{
	std::vector<Object> ObjectVector;
public:
	~ObjectArray() { ObjectVector.clear(); }

	void generateArray(std::vector<Object> &Other) { ObjectVector = Other; }

	bool isObjectExist(std::string objectID)
	{
		for (auto & i : ObjectVector)
		{
			if (i.getID() == objectID)
			{
				return true;
			}
		}

		return false;
	}
	Object getObject(std::string objectID)
	{
		for (auto i : ObjectVector)
		{
			if (i.getID() == objectID)
			{
				return i;
			}
		}
		return Object();
	}
	Object getObject(size_t Pos) { return ObjectVector[Pos]; }

	const std::vector<Object> &getObjects() { return ObjectVector; }
};