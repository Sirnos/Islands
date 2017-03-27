#include "Map.hpp"

void Map::fitMap()
{
	for (size_t i = 0; i < MapTiles.size(); i++)
	{
		MapTiles[i].shrink_to_fit();
	}
	MapTiles.shrink_to_fit();
}

Map::~Map()
{
	unloadMap();
}

void Map::unloadMap()
{
	for (size_t i = 0; i < MapTiles.size(); i++)
	{
		MapTiles[i].clear();
	}
	MapTiles.clear();
}

void Map::generateMap()
{
	sf::Vector2f beg(0, 0);

	noise::module::Perlin noiseModule;
	noiseModule.SetSeed(static_cast<int>(time(time_t(NULL))));
	noiseModule.SetOctaveCount(6);
	noiseModule.SetFrequency(1.0);
	noiseModule.SetPersistence(0.25);

	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		MapTiles.push_back(std::vector<Tile>(MAP_SIZE));
		for (size_t j = 0; j < MapTiles[i].size(); j++)
		{
			double var = noiseModule.GetValue(1.25 + (0.1 * i), 0.75 + (0.1 * j), 0.5);

			if (var > 0.75)
			{
				MapTiles[i][j].set(TILE_TYPE::ROCK, sf::Vector2f(beg));
			}
			else if (var > 0.5)
			{
				MapTiles[i][j].set(TILE_TYPE::DIRT, sf::Vector2f(beg));
			}
			else if (var > 0.2)
			{
				MapTiles[i][j].set(TILE_TYPE::GRASS, sf::Vector2f(beg));
			}
			else
			{
				MapTiles[i][j].set(TILE_TYPE::EMPTY, sf::Vector2f(beg));
			}

			beg.x += 64;
		}

		beg.y += 64;
		beg.x = 0;
	}
	fitMap();
}

void Map::bindTexturesToTiles(const sf::Texture *TileTextures,size_t SIZE)
{
	for (size_t i = 0; i < MapTiles.size(); i++)
	{
		for (size_t j = 0; j < MapTiles[i].size(); j++)
		{
			TILE_TYPE var = MapTiles[i][j].getTileType();

			switch (var)
			{
			case TILE_TYPE::ROCK :
				MapTiles[i][j].setTexture(const_cast<sf::Texture *>(TileTextures));
				break;
			case TILE_TYPE::DIRT :
				MapTiles[i][j].setTexture(const_cast<sf::Texture *>(TileTextures));
				break;
			case TILE_TYPE::GRASS :
				MapTiles[i][j].setTexture(const_cast<sf::Texture *>(TileTextures));
				break;
			default:
				break;
			}
		}
	}
}

const sf::Vector2i Map::getTiledPosition(sf::Vector2f characterPos)
{
	characterPos = characterPos / 64.0f;

	return static_cast<sf::Vector2i>(characterPos);
}

Tile * Map::getTile(sf::Vector2u tileNumber)
{
	return &MapTiles[tileNumber.x][tileNumber.y];
}
