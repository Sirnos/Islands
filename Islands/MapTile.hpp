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
inline TerrainType StringToTerrainType(std::string str)
{
	TerrainType ret = TerrainType::Null;
	if (str == "Dirt") { ret = TerrainType::Dirt; }
	else if (str == "Grass") { ret = TerrainType::Grass; }
	else if (str == "Sand") { ret = TerrainType::Sand; }
	else if (str == "Rock") { ret = TerrainType::Rock; }
	else if (str == "Water") { ret = TerrainType::Water; }

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


struct LocalMapVariablesDef
{
	TerrainType Biome;
	std::vector<std::pair<TerrainType, float>> TerrainTiles;
	std::vector<std::tuple<std::string, float, TerrainType>> SpawnableObjects;
	std::vector<std::pair<std::string, float>> SpawnableEntities;
	std::vector<std::pair<std::string, float >> SpawnableStructures;

	
	LocalMapVariablesDef() { Biome = TerrainType::Null; }
};

struct LocalMapVariables
{
	TerrainType Biome;
	std::vector<std::pair<TerrainType, float>> TerrainTiles;
	std::vector<std::tuple<size_t, float, TerrainType>> SpawnableObjects;
	std::vector<std::pair<size_t, float>> SpawnableEntities;
	std::vector<std::pair<size_t, float >> SpawnableStructures;

	LocalMapVariables() { Biome = TerrainType::Null; }
};