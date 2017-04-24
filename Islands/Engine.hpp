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

struct ObjTex
{
	unsigned Id;
	std::string Name;
	sf::Texture &Texture;

	void operator=(const ObjTex &Other)
	{
		Id = Other.Id;
		Name = Other.Name;
		Texture = Other.Texture;
	}
	ObjTex(unsigned id, std::string name, sf::Texture &refToTex)
		:Texture(refToTex)
	{
		Id = id; Name = name; 
	}
};

class Engine
{
	World GameWorld;
	Media mediaContainer;
	playerCharacter player;

	ObjectArray RawObjects;
	std::vector<ObjTex> ObjectsTextures;

	sf::View camera;

	void loadObjects();
	void checkPlayerBehaviour(IslandApp &app);
	bool checkPlayerPos();
	void spawnPlayer();


	void drawMap(IslandApp &app);
	void drawObjects(sf::RenderWindow &RWindow);
public:
	~Engine();

	void init();
	void operator()(IslandApp &app);

	void DrawAll(IslandApp &app);
};