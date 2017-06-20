#pragma once

#include "Map.hpp"
#include "ObjectsContainer.hpp"

class World
{
public:
	static const size_t WorldSize = 512;
private:

	Map WorldMap;
	ObjectContainer WorldObjects;

public:
	World();
	~World();

	void init(size_t Size = WorldSize);

	bool isPlaceImpassable(sf::Vector2f position);
	//bool isPositionImpasable(sf::Vector2i position);

	TILE getTile(sf::Vector2u tileIndex,bool safeVersion = true);
	unsigned getObject(sf::Vector2u objectIndex,bool safeVersion = true);

	bool setObject(sf::Vector2u objectIndex, unsigned objectId);
};