#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <utility>

#include "TerrainType.hpp"

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