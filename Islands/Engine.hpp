#pragma once

#include <SFML/Graphics.hpp>
#include "Tile.hpp"


class Engine
{
	//character player;
	//std::vector <character> mobs;

	sf::Sprite background;

	std::vector <Tile> playerIsland;
	std::vector <Tile> otherIslands;

	void sortPlayerIsland();
	void checkPlayerIslandIntegrity();

	std::vector <sf::RectangleShape *> drawableRectShapeVector;

public:
	~Engine();

	void init();

};