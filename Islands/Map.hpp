#pragma once

#include <noise/noise.h>
#include <vector>
#include <ctime>
#include "Tile.hpp"

class Map
{
	std::vector<std::vector<Tile>> MapTiles;

public:
	const static size_t MAP_SIZE = 256;

	~Map();

	void unloadMap();
	//void loadMapFromFile(std::string file);
	void generateMap();

	const sf::Vector2i getTiledPosition(sf::Vector2f characterPos);
	sf::Vector2u getMapSize();
	Tile *getTile(sf::Vector2u tileNumber);

	void fitMap();
};