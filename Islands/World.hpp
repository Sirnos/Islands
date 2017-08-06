#pragma once

#include "Map.hpp"

const size_t WorldSize = 512;

class World
{

	Map LocalMap;

public:
	World(){}
	~World(){}

	void init(size_t Size = WorldSize)
	{
		LocalMap.generateMap(Size);
	}

	bool isPlaceImpassable(sf::Vector2f position)
	{
		sf::Vector2u uPos = static_cast<sf::Vector2u>(Map::getTiledPosition(position));
		if(uPos.x >= 0 && uPos.y >= 0 && uPos.x < WorldSize && uPos.y < WorldSize)
		if (getTileTerrain(uPos) == TerrainType::Null) { return true; }
		return false;
	}
	//bool isPositionImpasable(sf::Vector2i position);

	TerrainType getTileTerrain(sf::Vector2u tileIndex)
	{
		return LocalMap.getTileTerrain(tileIndex);
	}

	void setObject(sf::Vector2u objectIndex, Object* newObject)
	{
		if (LocalMap.getTileObject(objectIndex) != nullptr)
		{
			LocalMap.clearTileObject(objectIndex);
			LocalMap.setTileObject(objectIndex, newObject);
		}
		else
		{
			LocalMap.setTileObject(objectIndex,newObject);
		}
	}
	bool placeObject(sf::Vector2u objectIndex, unsigned objectId,std::vector<ObjectDef*>& ObjectsDefs,sf::Time AtTime)
	{
		if (LocalMap.getTileObject(objectIndex) == nullptr)
		{
			ObjectType objType= ObjectsDefs[objectId]->getType();

			switch (objType)
			{
			case ObjectType::Default:
				LocalMap.setTileObject(objectIndex, new Object(objectId));
				break;
			case ObjectType::Chest:
				LocalMap.setTileObject(objectIndex, new ChestObject(objectId,
					dynamic_cast<ChestDef*>(ObjectsDefs[objectId])->getCapacity()));
				break;
			case ObjectType::Tree:
				break;
			case ObjectType::Sapling:
				LocalMap.setTileObject(objectIndex, new SaplingObject(objectId, AtTime.asSeconds()));
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
		LocalMap.clearTileObject(objectIndex);
	}

	Object* getObject(sf::Vector2u objectIndex)
	{
		return LocalMap.getTileObject(objectIndex);
	}
	ObjectType getObjectType(sf::Vector2u objectIndex)
	{
		if (LocalMap.getTileObject(objectIndex) == nullptr)
		{
			return ObjectType::Default;
		}
		return LocalMap.getTileObject(objectIndex)->type;
	}
	unsigned getObjectId(sf::Vector2u objectIndex)
	{
		if (LocalMap.getTileObject(objectIndex) == nullptr)
		{
			return 0;
		}
		return LocalMap.getTileObject(objectIndex)->Id;
	}
};