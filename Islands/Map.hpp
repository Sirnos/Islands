#pragma once

#include <SFML/System/Clock.hpp>
#include <noise/noise.h>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <ctime>
#include "MapTile.hpp"
#include "ErrorHandler.hpp"

class Map
{
	std::vector<std::vector<MapTile>> Tiles;
public:
	~Map() { Tiles.clear(); }

	//void loadMapFromFile(std::string file);
	void generateMap(size_t Size)
	{
		sf::Clock TestClock;

		noise::module::Perlin noiseModule;
		noiseModule.SetSeed(static_cast<int>(time(time_t(NULL))));
		noiseModule.SetOctaveCount(6);
		noiseModule.SetFrequency(1.0);
		noiseModule.SetPersistence(0.25);

		Tiles.resize(Size, std::vector<MapTile>(Size));

		double mnX = 0;
		for (auto & i : Tiles)
		{
			double mnY = 0;
			for (auto & j : i)
			{
				double noise = noiseModule.GetValue(1.25 + (0.1 * mnX), 0.75 + (0.1 * mnY), 0.5);
				if (noise > 0.75)
				{
					j.Terrain = TerrainType::Rock;
				}
				else if (noise > 0.35)
				{
					j.Terrain = TerrainType::Dirt;
				}
				else if (noise > -0.25)
				{
					j.Terrain = TerrainType::Grass;
				}
				else
				{
					j.Terrain = TerrainType::Water;
				}
				mnY += 1;
			}
			mnX += 1;
		}

		for (auto & i : Tiles)
		{
			i.shrink_to_fit();
		}
		Tiles.shrink_to_fit();

		ErrorHandler::log("Generate Local Map with size " + std::to_string(getMapSize()) +
			" In " + std::to_string(TestClock.getElapsedTime().asMilliseconds()) + " milisecs ");
	}

	static sf::Vector2i getTiledPosition(sf::Vector2f characterPos)
	{
		characterPos = characterPos / TILE_SIZE;
		return static_cast<sf::Vector2i>(characterPos);
	}
	static sf::Vector2f getNormalPosition(sf::Vector2i tileNumber)
	{
		return sf::Vector2f(static_cast<float>(tileNumber.x) * TILE_SIZE,
			static_cast<float>(tileNumber.y) * TILE_SIZE);
	}

	size_t getMapSize()
	{
		return Tiles.size();
	}
	TerrainType getTileTerrain(sf::Vector2u tileNumber)
	{
		return Tiles[tileNumber.x][tileNumber.y].Terrain;
	}

	Object* getTileObject(sf::Vector2u index)
	{
		return Tiles[index.x][index.y].TileObject;
	}
	void setTileObject(sf::Vector2u index, Object* object)
	{
		Tiles[index.x][index.y].TileObject = object;
	}
	void clearTileObject(sf::Vector2u index)
	{
		if (Tiles[index.x][index.y].TileObject != nullptr)
		{
			delete Tiles[index.x][index.y].TileObject;
			setTileObject(index, nullptr);
		}
	}
};