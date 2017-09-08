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
	std::vector<std::pair<std::string, unsigned >> SpawnableStructures;


	LocalMapVariablesDef()
		:Biome(TerrainType::Null)
	{}
	~LocalMapVariablesDef() = default;
};

struct LocalMapVariables
{
	TerrainType Biome;
	std::vector<std::pair<TerrainType, float>> TerrainTiles;
	std::vector<std::tuple<size_t, float, TerrainType>> SpawnableObjects;
	std::vector<std::pair<size_t, float>> SpawnableEntities;
	std::vector<std::pair<size_t, unsigned >> SpawnableStructures;

	LocalMapVariables()
		:Biome(TerrainType::Null)
	{}
	~LocalMapVariables() = default;
};