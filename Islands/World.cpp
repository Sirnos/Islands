#include "World.hpp"

World::World() {}

World::~World()	{}

void World::init(size_t Size)
{
	WorldMap.generateMap();
}

bool World::isPlaceImpassable(sf::Vector2f position)
{
	if (position.x < 0 || position.y < 0) { return true; }

	sf::Vector2u uPos = static_cast<sf::Vector2u>(Map::getTiledPosition(position));
	if (getTile(uPos) == TILE_TYPE::EMPTY) { return true; }
	return false;
}

TILE_TYPE World::getTile(sf::Vector2u tileIndex, bool safeVersion)
{
	if (safeVersion == true)
	{
		if (tileIndex.x < WorldSize && tileIndex.y < WorldSize)
		{
			return WorldMap.getTile(tileIndex)->getTileType();
		}

		return TILE_TYPE();
	}

	return WorldMap.getTile(tileIndex)->getTileType();
}

unsigned World::getObject(sf::Vector2u objectIndex, bool safeVersion)
{
	if (safeVersion == true)
	{
		if (objectIndex.x < WorldSize && objectIndex.y < WorldSize)
		{
			return WorldObjects.getElementID(objectIndex);
		}

		return 0;
	}

	return WorldObjects.getElementID(objectIndex);
}
