#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "IslandApp.hpp"
#include "playerCharacter.hpp"
#include "Media.hpp"
#include "character.hpp"
#include "Map.hpp"
#include "ObjectArray.hpp"
#include "Log.hpp"

class Engine
{
	Map GameMap;
	Media mediaContainer;
	playerCharacter player;
	ObjectArray Objects;

	sf::View camera;
	sf::Sprite background;

	std::vector < std::pair <Character*, size_t>> mobs;

	void loadObjects();
	void checkPlayerBehaviour(IslandApp &app);
	bool checkPlayerPos();
	void spawnPlayer();

public:
	~Engine();

	void init();
	void operator()(IslandApp &app);

	void drawMap(IslandApp &app);
	void DrawAll(IslandApp &app);
};