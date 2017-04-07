#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "playerCharacter.hpp"
#include "Media.hpp"
#include "character.hpp"
#include "Map.hpp"

#include "Log.hpp"

class Engine
{
	Map GameMap;
	Media mediaContainer;
	playerCharacter player;

	sf::View camera;
	sf::Sprite background;

	std::vector < std::pair <Character*, size_t>> mobs;

	void checkPlayerBehaviour(sf::RenderWindow *window);
	bool checkPlayerPos();
	void spawnPlayer();

public:
	~Engine();

	void init();
	void operator()(sf::RenderWindow * window);

	void drawMap(sf::RenderWindow * window);
	void DrawAll(sf::RenderWindow * window);
};