#pragma once

#include <SFML/Graphics.hpp>

#include "playerCharacter.hpp"
#include "Tile.hpp"
#include "Media.hpp"
#include "object.hpp"

class Engine
{
	Media mediaContainer;

	playerCharacter player;
	//std::vector <character> mobs;

	//std::vector < std::pair <character*, size_t> mobs;

	sf::Sprite background;

	std::vector <std::vector<Tile>> playerIsland;
	std::vector <object> objects;

public:
	~Engine();

	void init();
	void operator()();

	void DrawAll(sf::RenderWindow *window);

};