#pragma once

#include "Object.hpp"

const float TILE_SIZE = 64;

enum class TerrainType
{
	Null,
	Dirt,
	Grass,
	Sand,
	Rock,
	Water
};

struct MapTile
{
	TerrainType Terrain;
	Object * TileObject;

	MapTile()
	{
		Terrain = TerrainType::Null;
		TileObject = nullptr;
	}
	~MapTile()
	{
		if(TileObject != nullptr){ delete TileObject; }
	}
};