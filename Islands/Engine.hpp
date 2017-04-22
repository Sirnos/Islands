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
	std::map<std::string,std::pair<unsigned,sf::Texture&>> ObjectsTextures;

	sf::View camera;

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