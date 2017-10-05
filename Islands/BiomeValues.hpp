#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <utility>

#include "TerrainType.hpp"

struct BiomeValuesDef
{
	TerrainType BiomeName;
	std::vector<std::pair<TerrainType, float>> TerrainTiles;
	std::vector<std::tuple<std::string, float, TerrainType>> SpawnableObjects;
	std::vector<std::pair<std::string, float>> SpawnableEntities;
	std::vector<std::pair<std::string, unsigned >> SpawnableStructures;


	BiomeValuesDef()
		:BiomeName(TerrainType::Null)
	{}
	~BiomeValuesDef() = default;
};

struct BiomeValues
{
	TerrainType BiomeName;
	std::vector<std::pair<TerrainType, float>> TerrainTiles;
	std::vector<std::tuple<size_t, float, TerrainType>> SpawnableObjects;
	std::vector<std::pair<size_t, float>> SpawnableEntities;
	std::vector<std::pair<size_t, unsigned >> SpawnableStructures;

	BiomeValues()
		:BiomeName(TerrainType::Null)
	{}
	~BiomeValues() = default;
};