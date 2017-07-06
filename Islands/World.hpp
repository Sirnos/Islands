#pragma once

#include "Map.hpp"
#include "ObjectMap.hpp"

const size_t WorldSize = 512;

class World
{

	Map WorldMap;
	ObjectMap WorldObjects;

public:
	World(){}
	~World(){}

	void init(size_t Size = WorldSize)
	{
		WorldMap.generateMap(Size);
		WorldObjects.generate(Size);
	}

	bool isPlaceImpassable(sf::Vector2f position)
	{
		if (position.x < 0 || position.y < 0) { return true; }

		sf::Vector2u uPos = static_cast<sf::Vector2u>(Map::getTiledPosition(position));
		if (getTile(uPos) == TILE::EMPTY) { return true; }
		return false;
	}
	//bool isPositionImpasable(sf::Vector2i position);

	TILE getTile(sf::Vector2u tileIndex, bool safeVersion = true)
	{
		if (safeVersion == true)
		{
			if (tileIndex.x < WorldSize && tileIndex.y < WorldSize)
			{
				return WorldMap.getTile(tileIndex);
			}

			return TILE();
		}

		return WorldMap.getTile(tileIndex);
	}

	/*
	bool setObject(sf::Vector2u objectIndex, unsigned objectId)
	{
		if (WorldObjects.getElementID(objectIndex) != 0) { return false; }

		WorldObjects.pushNewObject(objectId, objectIndex);
		return true;
	}
	*/

	ObjectType getObjectType(sf::Vector2u objectIndex)
	{
		if (WorldObjects.getObject(objectIndex) == nullptr)
		{
			return ObjectType::Default;
		}
		return WorldObjects.getObject(objectIndex)->type;
	}
	unsigned getObjectId(sf::Vector2u objectIndex)
	{
		if (WorldObjects.getObject(objectIndex) == nullptr)
		{
			return 0;
		}
		return WorldObjects.getObject(objectIndex)->Id;
	}
};