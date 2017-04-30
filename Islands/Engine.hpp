#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "IslandApp.hpp"
#include "playerCharacter.hpp"
#include "Media.hpp"
#include "ObjectArray.hpp"
#include "Log.hpp"
#include "World.hpp"

class Engine
{
	World GameWorld;
	Media mediaContainer;
	playerCharacter player;

	ObjectArray RawObjects;

	sf::View camera;

	void loadObjects();
	void checkPlayerBehaviour(IslandApp &app);
	bool checkPlayerPos();
	void spawnPlayer();


	void drawTile(sf::Vector2u tileIndex, sf::RenderWindow &window, sf::RectangleShape &shp);
	void drawObject(sf::Vector2u objectIndex, sf::RenderWindow &window, sf::RectangleShape &shp);

	void drawWorld(IslandApp &app);
public:
	~Engine();

	void init();
	void operator()(IslandApp &app);

	void DrawAll(IslandApp &app);
};