#pragma once

const float TILE_SIZE = 64;

enum class TILE_TYPE : unsigned
{
	EMPTY,
	DIRT,
	GRASS,
	ROCK,
	BRIGDE
};

class Tile
{
	TILE_TYPE type;
public:
	Tile()
	{
		type = TILE_TYPE::EMPTY;
	}
	Tile(TILE_TYPE newTileType)
	{
		type = newTileType;
	}

	TILE_TYPE getTileType()
	{
		return type;
	}

	void setType(TILE_TYPE newtype)
	{
		type = newtype;
	}

};