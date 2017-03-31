#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

const float TILE_SIZE = 64;

enum class TILE_TYPE :unsigned
{
	EMPTY,
	DIRT,
	GRASS,
	ROCK,
	BRIGDE
};

class Tile
{
	sf::Vector2f position;
	TILE_TYPE type;
	unsigned HP;

public:
	Tile()
	{}
	Tile(TILE_TYPE newTileType,sf::Vector2f newTilePos)
	{
		type = newTileType;
		position = newTilePos;

		switch (newTileType)
		{
		case TILE_TYPE::ROCK :
			HP = 9;
			break;
		case TILE_TYPE::GRASS :
			HP = 6;
			break;
		case TILE_TYPE::DIRT :
			HP = 3;
		}
	}

	sf::Vector2f getPosition()
	{
		return position;
	}
	TILE_TYPE getTileType()
	{
		return type;
	}

	void moveTile(sf::Vector2f offset)
	{
		position += offset;
	}
	void setTilePosition(sf::Vector2f newPosition)
	{
		position = newPosition;
	}

	unsigned getTileHP()
	{
		return HP;
	}
	void applyDamageToTile(unsigned dmg)
	{
		HP -= dmg;
	}

	void set(TILE_TYPE newtype, sf::Vector2f pos)
	{
		type = newtype;
		position = pos;

		switch (newtype)
		{
		case TILE_TYPE::BRIGDE:
			HP = 4;
			break;
		case TILE_TYPE::ROCK:
			HP = 9;
			break;
		case TILE_TYPE::GRASS:
			HP = 6;
			break;
		case TILE_TYPE::DIRT:
			HP = 3;
			break;
		}
	}
	void setTileType(TILE_TYPE newType)
	{
		set(newType, position);
	}
};