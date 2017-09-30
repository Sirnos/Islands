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


	size_t getWorldMapSize() const { return WorldMap.size(); }
	size_t getLocalMapSize() const { return LocalMap.size(); }


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


	TerrainType getWorldMapTileTerrain(const sf::Vector2u &tile) const { return WorldMap[tile.x][tile.y]; }
	TerrainType getLocalMapTileTerrain(const sf::Vector2u &tile) const { return LocalMap[tile.x][tile.y].Terrain; }


	void setWorldMapTileTerrain(const sf::Vector2u &tile, const TerrainType &Terrain)
	{
		WorldMap[tile.x][tile.y] = Terrain;
	}
	void setLocalMapTileTerrain(const sf::Vector2u &tile, const TerrainType &Terrain)
	{
		LocalMap[tile.x][tile.y].Terrain = Terrain;
	}


	Object* const getLocalMapTileObject(const sf::Vector2u &tile) const
	{
		return LocalMap[tile.x][tile.y].TileObject;
	}
	unsigned getLocalMapTileObjectId(const sf::Vector2u &tile) const
	{
		if (LocalMap[tile.x][tile.y].TileObject != nullptr)
		{
			return LocalMap[tile.x][tile.y].TileObject->getId();
		}
		return 0;
	}
	

	void removeLocalMapTileObject(const sf::Vector2u &tile)
	{
		if (LocalMap[tile.x][tile.y].TileObject != nullptr)
		{
			delete LocalMap[tile.x][tile.y].TileObject;
			LocalMap[tile.x][tile.y].TileObject = nullptr;
		}
	}
	void setLocalMapTileObject(const sf::Vector2u &tile, Object* objectToSet)
	{
		LocalMap[tile.x][tile.y].TileObject = objectToSet;
	}


	static sf::Vector2i getTiledPosition(const sf::Vector2f &characterPos)
	{
		sf::Vector2f tiledPos = characterPos / TILE_SIZE;
		return static_cast<sf::Vector2i>(tiledPos);
	}
	static sf::Vector2f getNormalPosition(const sf::Vector2i &tileNumber)
	{
		return sf::Vector2f(static_cast<float>(tileNumber.x) * TILE_SIZE,
			static_cast<float>(tileNumber.y) * TILE_SIZE);
	}
};