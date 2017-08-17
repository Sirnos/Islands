#pragma once

#include "Map.hpp"

const size_t DefaultWorldSize = 512;

class World
{
	Map LocalMap;

public:
	void Generate(size_t LocalMapSize = DefaultWorldSize)
	{
		LocalMap.generateMap(LocalMapSize);
	}

	bool isPlaceImpassable(sf::Vector2f position)
	{
		sf::Vector2u uPos = static_cast<sf::Vector2u>(Map::getTiledPosition(position));
		if(uPos.x >= 0 && uPos.y >= 0 && uPos.x < getLocalMapSize() && uPos.y < getLocalMapSize())
		if (getTileTerrain(uPos) == TerrainType::Null) { return true; }
		return false;
	}

	sf::IntRect getTileCollisionBox(sf::Vector2u tile,const std::vector<ObjectDef*> Def)
	{
		sf::IntRect collideObject;
		unsigned collideObjectId = getObjectId(tile);
		if (collideObjectId != 0)
		{
			sf::FloatRect collideObjectBox = Def[collideObjectId]->getCollisionBox();
			if (collideObjectBox.top <= 0.01f && collideObjectBox.left <= 0.01f
				&& collideObjectBox.height <= 0.01f && collideObjectBox.width <= 0.01f)
			{
				return collideObject;
			}
			collideObject.top = tile.y * static_cast<int>(TILE_SIZE);
			collideObject.left = tile.x * static_cast<int>(TILE_SIZE);
			collideObject.width = static_cast<int>(TILE_SIZE);
			collideObject.height = static_cast<int>(TILE_SIZE);

			if (collideObjectBox.top < 1.0f)
			{
				collideObject.top += static_cast<int>(TILE_SIZE * collideObjectBox.top);
			}
			if (collideObjectBox.left < 1.0f)
			{
				collideObject.left += static_cast<int>(TILE_SIZE * collideObjectBox.left);
			}
			if (collideObjectBox.height < 1.0f)
			{
				collideObject.height -= static_cast<int>(TILE_SIZE * collideObjectBox.height);
			}
			if (collideObjectBox.width < 1.0f)
			{
				collideObject.width -= static_cast<int>(TILE_SIZE * collideObjectBox.width);
			}
		}
		return collideObject;
	}

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

	size_t getLocalMapSize() { return LocalMap.getMapSize(); }
};