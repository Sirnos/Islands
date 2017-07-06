#pragma once


#include "Object.hpp"
#include <vector>

class ObjectMap
{
	std::vector<std::vector<Object*>> ObjectsMap;

public:
	ObjectMap(){}
	~ObjectMap()
	{
		for (size_t i = 0; i < ObjectsMap.size(); i++)
		{
			for (size_t j = 0; j < ObjectsMap[i].size(); j++)
			{
				delete ObjectsMap[i][j];
			}
		}
		ObjectsMap.clear();
	}

	void generate(size_t Size)
	{
		ObjectsMap.resize(Size, std::vector<Object*>(Size, nullptr));
	}

	Object* getObject(sf::Vector2u index)
	{
		return ObjectsMap[index.x][index.y];
	}
	void setObject(sf::Vector2u index, Object* object)
	{
		ObjectsMap[index.x][index.y] = object;
	}
	void clearObject(sf::Vector2u index)
	{
		if (ObjectsMap[index.x][index.y] != nullptr)
		{
			delete ObjectsMap[index.x][index.y];
		}
	}

	sf::Vector2u getSize()
	{
		return sf::Vector2u(ObjectsMap.size(), ObjectsMap[0].size());
	}
};