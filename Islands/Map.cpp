#include "Map.hpp"

void Map::fitMap()
{
	for (size_t i = 0; i < TilesMap.size(); i++)
	{
		TilesMap[i].shrink_to_fit();
	}
	TilesMap.shrink_to_fit();

	for (size_t i = 0; i < ObjectMap.size(); i++)
	{
		ObjectMap[i].shrink_to_fit();
	}
	ObjectMap.shrink_to_fit();
}

Map::~Map()
{
	unloadMap();
}

void Map::unloadMap()
{
	for (size_t i = 0; i < TilesMap.size(); i++)
	{
		TilesMap[i].clear();
	}
	TilesMap.clear();

	for (size_t i = 0; i < ObjectMap.size(); i++)
	{
		ObjectMap[i].clear();
	}
	ObjectMap.clear();
}

void Map::generateMap()
{
	noise::module::Perlin noiseModule;
	noiseModule.SetSeed(static_cast<int>(time(time_t(NULL))));
	noiseModule.SetOctaveCount(6);
	noiseModule.SetFrequency(1.0);
	noiseModule.SetPersistence(0.25);

	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		ObjectMap.push_back(std::vector<Object>(MAP_SIZE));
		TilesMap.push_back(std::vector<Tile>(MAP_SIZE));
		for (size_t j = 0; j < TilesMap[i].size(); j++)
		{
			double var = noiseModule.GetValue(1.25 + (0.1 * i), 0.75 + (0.1 * j), 0.5);

			if (var > 0.75)
			{
				TilesMap[i][j].setType(TILE_TYPE::ROCK);
			}
			else if (var > 0.5)
			{
				TilesMap[i][j].setType(TILE_TYPE::DIRT);
			}
			else if (var > 0.05)
			{
				TilesMap[i][j].setType(TILE_TYPE::GRASS);
			}
			else
			{
				TilesMap[i][j].setType(TILE_TYPE::EMPTY);
			}
		}
	}
}

const sf::Vector2i Map::getTiledPosition(sf::Vector2f characterPos)
{
	characterPos = characterPos / TILE_SIZE;

	return static_cast<sf::Vector2i>(characterPos);
}

const sf::Vector2f Map::getNormalPosition(sf::Vector2i tileNumber)
{
	return sf::Vector2f(static_cast<float>(tileNumber.x) * TILE_SIZE,
		static_cast<float>(tileNumber.y) * TILE_SIZE);
}

sf::Vector2u Map::getMapSize()
{
	return sf::Vector2u(TilesMap.size(), TilesMap[0].size());
}

Tile * Map::getTile(sf::Vector2u tileNumber)
{
	return &TilesMap[tileNumber.x][tileNumber.y];
}

Object *Map::getObject(sf::Vector2u ObjectNumber)
{
	return &ObjectMap[ObjectNumber.x][ObjectNumber.y];
}