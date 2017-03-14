#pragma once

#include <SFML/Graphics.hpp>

class Engine
{
	//character player;
	//std::vector <character> mobs;

	sf::Sprite background;

	std::vector <std::vector <TILE_TYPE> > island;

	std::vector <TILE_TYPE> otherIslands;
public:
	~Engine();
};