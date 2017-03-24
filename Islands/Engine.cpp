#include "Engine.hpp"

void Engine::cleanIslands()
{
	
}

Engine::~Engine()
{
	playerIsland.clear();
	otherIslands.clear();

	drawableRectShapeVector.clear();
}

void Engine::init()
{
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			playerIsland.push_back(Tile(TILE_TYPE::DIRT, sf::Vector2f(i * 64, j * 64)));
		}
	}
}
