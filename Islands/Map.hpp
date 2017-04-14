#pragma once

#include <noise/noise.h>
#include <SFML/System.hpp>
#include <vector>
#include <ctime>
#include "Tile.hpp"
#include "Log.hpp"

class Map
{
public:
	const static size_t MAP_SIZE = 512;
private:
	std::vector<std::vector<Tile>> TilesMap;

public:
	~Map();

	void unloadMap();
	//void loadMapFromFile(std::string file);
	void generateMap();

	const static sf::Vector2i getTiledPosition(sf::Vector2f characterPos);
	const static sf::Vector2f getNormalPosition(sf::Vector2i tileNumber);

	sf::Vector2u getMapSize();
	Tile *getTile(sf::Vector2u tileNumber);

	void fitMap();
};