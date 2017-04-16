#include "Map.hpp"

void Map::fitMap()
{
	for (auto & i : TilesMap)
	{
		i.shrink_to_fit();
	}
	TilesMap.shrink_to_fit();
}

Map::~Map()
{
	unloadMap();
}

void Map::unloadMap()
{
	TilesMap.clear();
}

void Map::generateMap()
{
	noise::module::Perlin noiseModule;
	noiseModule.SetSeed(static_cast<int>(time(time_t(NULL))));
	noiseModule.SetOctaveCount(6);
	noiseModule.SetFrequency(1.0);
	noiseModule.SetPersistence(0.25);

	TilesMap.resize(MAP_SIZE,std::vector<Tile>(MAP_SIZE));

	double mnX = 0;
	for (auto & i : TilesMap)
	{
		double mnY = 0;
		for (auto & j : i)
		{
			double TileValue = noiseModule.GetValue(1.25 + (0.1 * mnX), 0.75 + (0.1 * mnY), 0.5);
			if (TileValue > 0.75)
			{
				j.setType(TILE_TYPE::ROCK);
			}
			else if (TileValue > 0.5)
			{
				j.setType(TILE_TYPE::DIRT);
			}
			else if (TileValue > 0.05)
			{
				j.setType(TILE_TYPE::GRASS);
			}
			else
			{
				j.setType(TILE_TYPE::EMPTY);
			}
			mnY += 1;
		}
		mnX += 1;
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