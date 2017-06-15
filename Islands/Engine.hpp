#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "IslandApp.hpp"

#include "PlayerGui.hpp"

#include "GameComponentsLoader.hpp"
#include "GameComponentsUnloader.hpp"

#include "ItemDefContainer.hpp"
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

	PlayerGui GameGui;

	ObjectArray RawObjects;
	ItemDefContainer Items;

	sf::View camera;

	void loadGameComponents();
	void checkPlayerBehaviour(IslandApp &app);
	bool checkPlayerPos();
	void spawnPlayer();


	void drawTile(sf::Vector2u tileIndex, sf::RenderWindow &window, sf::RectangleShape &shp);
	void drawObject(sf::Vector2u objectIndex, sf::RenderWindow &window, sf::RectangleShape &shp);

	void drawWorld(IslandApp &app);

	void drawPlayerGui(IslandApp & app);
public:
	~Engine();

	void init();
	void operator()(IslandApp &app,char key,mouseWheel last);

	void DrawAll(IslandApp &app);
};