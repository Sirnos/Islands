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

inline std::string TerrainTypeToString(TerrainType terrain)
{
	std::string ret("Terrain::");
	switch (terrain)
	{
	case TerrainType::Null:
		ret += "Null";
		break;
	case TerrainType::Dirt:
		ret += "Dirt";
		break;
	case TerrainType::Grass:
		ret += "Grass";
		break;
	case TerrainType::Sand:
		ret += "Sand";
		break;
	case TerrainType::Rock:
		ret += "Rock";
		break;
	case TerrainType::Water:
		ret += "Water";
		break;
	default:
		ret += "!IsUndefided";
		break;
	}
	return ret;
}

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