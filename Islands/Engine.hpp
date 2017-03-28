#pragma once

#include <SFML/Graphics.hpp>

#include "playerCharacter.hpp"
#include "Media.hpp"
#include "object.hpp"
#include "character.hpp"
#include "Map.hpp"

class Engine
{
	Map GameMap;
	Media mediaContainer;

	playerCharacter player;
	std::vector < std::pair <Character*, size_t>> mobs;

	sf::Sprite background;
	std::vector <object> objects;

	sf::View camera;
public:
	~Engine();

	void init();
	void operator()();

	void drawMap(sf::RenderWindow * window);
	void DrawAll(sf::RenderWindow *window);
};