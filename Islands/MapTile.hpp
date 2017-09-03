#pragma once

#include "Object.hpp"
#include "TerrainType.hpp"

struct MapTile
{
	TerrainType Terrain;
	Object * TileObject;

	MapTile()
	{
		Terrain = TerrainType::Null;
		TileObject = nullptr;
	}
	MapTile(const MapTile & other)
		:Terrain(other.Terrain), TileObject(nullptr){}
	~MapTile()
	{
		if(TileObject != nullptr){ delete TileObject; }
	}
};