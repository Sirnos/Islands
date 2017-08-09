#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "IslandApp.hpp"

#include "ErrorHandler.hpp"
#include "CollisionDetect.hpp"

#include "Gui.hpp"

#include "GameComponentsLoader.hpp"

#include "DefContainer.hpp"
#include "LyingItemsContainer.hpp"
#include "CraftingSystem.hpp"

#include "playerCharacter.hpp"

#include "Media.hpp"
#include "World.hpp"

#include "Console.hpp"

class Engine
{
	unsigned TileDrawRange;

	World GameWorld;
	Media mediaContainer;
	playerCharacter player;

	Gui GameGui;

	CraftingSystem Crafting;
	ObjectDefContainer Objects;
	ItemDefContainer Items;
	LyingItemsContainer LyingItems;

	sf::Clock GameClock;
	Console GameConsole;

	void loadGameComponents();

	void checkPlayerEnvironment();
	void checkPlayerBehaviour(IslandApp &app);
	bool checkPlayerPos();

	void spawnPlayer();

	void checkGuiOperations(EquipmentType type, sf::Vector2u field);

	void drawConsole(IslandApp &app);

	void drawTile(sf::Vector2u tileIndex, sf::RenderWindow &window, sf::RectangleShape &shp);
	void drawObject(sf::Vector2u objectIndex, sf::RenderWindow &window, sf::RectangleShape &shp);

	void drawWorld(IslandApp &app);

	void drawPlayerGui(IslandApp & app);
	void pushItemTextureToRect(sf::Vector2f pos, unsigned itemId,sf::RectangleShape &rect);
public:
	Engine(unsigned LocalMapSize, unsigned MaxNumberOfLyingItems, unsigned PlayerPickUpItemsRange, unsigned MaxTileDrawRange);
	~Engine();

	void operator()(IslandApp &app,char key,mouseWheel last,bool isMouseClick);

	void manageConsole(sf::Event &event, sf::Vector2f mousePos, bool isMouseRClick);

	void DrawAll(IslandApp &app);
};