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

	TILE getTile(sf::Vector2u tileIndex)
	{
		return WorldMap.getTile(tileIndex);
	}

	void setObject(sf::Vector2u objectIndex, Object* newObject)
	{
		if (WorldObjects.getObject(objectIndex) != nullptr)
		{
			WorldObjects.clearObject(objectIndex);
			WorldObjects.setObject(objectIndex, newObject);
		}
		else
		{
			WorldObjects.setObject(objectIndex,newObject);
		}
	}
	bool placeObject(sf::Vector2u objectIndex, unsigned objectId,std::vector<ObjectDef*>& ObjectsDefs,sf::Time AtTime)
	{
		if (WorldObjects.getObject(objectIndex) == nullptr)
		{
			ObjectType objType= ObjectsDefs[objectId]->getType();

			switch (objType)
			{
			case ObjectType::Default:
				WorldObjects.setObject(objectIndex, new Object(objectId));
				break;
			case ObjectType::Chest:
				WorldObjects.setObject(objectIndex, new ChestObject(objectId,
					dynamic_cast<ChestDef*>(ObjectsDefs[objectId])->getCapacity()));
				break;
			case ObjectType::Tree:
				break;
			case ObjectType::Sapling:
				WorldObjects.setObject(objectIndex, new SaplingObject(objectId, AtTime.asSeconds()));
				break;
			case ObjectType::Spawner:
				break;
			default:
				break;
			}
			return true;
		}
		return false;
	}
	void clearObject(sf::Vector2u objectIndex)
	{
		WorldObjects.clearObject(objectIndex);
	}

	Object* getObject(sf::Vector2u objectIndex)
	{
		return WorldObjects.getObject(objectIndex);
	}
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