#pragma once

#include <vector>

#include "MapTile.hpp"


class World
{
	std::vector<std::vector<TerrainType>> WorldMap;
	std::vector<std::vector<MapTile>> LocalMap;

public:
	World() = default;
	World(const World & other) = delete;
	~World() = default;

	const static size_t DefaultMapSize = 512;
	const static size_t DefaultWorldSize = 256;


	size_t getWorldMapSize() { return WorldMap.size(); }
	size_t getLocalMapSize() { return LocalMap.size(); }


	void resizeWorldMap(size_t Size = DefaultWorldSize)
	{
		WorldMap.resize(Size, std::vector<TerrainType>(Size, TerrainType::Null));
		WorldMap.shrink_to_fit();
	}
	void resizeLocalMap(size_t Size = DefaultMapSize)
	{
		LocalMap.resize(Size, std::vector<MapTile>(Size, MapTile()));
		LocalMap.shrink_to_fit();
	}


	TerrainType getWorldMapTileTerrain(sf::Vector2u tile) { return WorldMap[tile.x][tile.y]; }
	TerrainType getLocalMapTileTerrain(sf::Vector2u tile) { return LocalMap[tile.x][tile.y].Terrain; }

	void setWorldMapTileTerrain(sf::Vector2u tile, TerrainType Terrain)
	{
		WorldMap[tile.x][tile.y] = Terrain;
	}
	void setLocalMapTileTerrain(sf::Vector2u tile, TerrainType Terrain)
	{
		LocalMap[tile.x][tile.y].Terrain = Terrain;
	}

	Object* const getLocalMapTileObject(sf::Vector2u tile)
	{
		return LocalMap[tile.x][tile.y].TileObject;
	}
	unsigned getLocalMapTileObjectId(sf::Vector2u tile)
	{
		if (LocalMap[tile.x][tile.y].TileObject != nullptr)
		{
			return LocalMap[tile.x][tile.y].TileObject->Id;
		}
		return 0;
	}
	
	void removeLocalMapTileObject(sf::Vector2u tile)
	{
		if (LocalMap[tile.x][tile.y].TileObject != nullptr)
		{
			delete LocalMap[tile.x][tile.y].TileObject;
			LocalMap[tile.x][tile.y].TileObject = nullptr;
		}
	}
	void setLocalMapTileObject(sf::Vector2u tile, Object* objectToSet)
	{
		LocalMap[tile.x][tile.y].TileObject = objectToSet;
	}


	static sf::Vector2i getTiledPosition(sf::Vector2f characterPos)
	{
		characterPos = characterPos / TILE_SIZE;
		return static_cast<sf::Vector2i>(characterPos);
	}
	static sf::Vector2f getNormalPosition(sf::Vector2i tileNumber)
	{
		return sf::Vector2f(static_cast<float>(tileNumber.x) * TILE_SIZE,
			static_cast<float>(tileNumber.y) * TILE_SIZE);
	}
};