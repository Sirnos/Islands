#pragma once

#include <SFML/Graphics.hpp>

#include "IslandApp.hpp"

#include "WorldManager.hpp"

#include "Gui.hpp"

#include "GameComponentsLoader.hpp"

#include "LyingItemsContainer.hpp"
#include "CraftingSystem.hpp"

#include "PlayerEntity.hpp"

#include "Media.hpp"

#include "Console.hpp"

#include "EngineVars.hpp"

class Engine
{
	unsigned TileDrawRange;

	WorldManager GWorldManager;

	std::shared_ptr<World> GameWorld{ new World };
	std::shared_ptr<ObjectDefContainer> Objects{ new ObjectDefContainer };
	std::shared_ptr<ItemDefContainer> Items{ new ItemDefContainer };
	std::shared_ptr<MonsterDefContainer> Entities{ new MonsterDefContainer };


	Media mediaContainer;
	PlayerEntity Player;

	Gui GameGui;

	CraftingSystem Crafting;
	LyingItemsContainer LyingItems;

	sf::Clock GameClock;
	Console GameConsole;

	void loadGameComponents();

	void checkPlayerEnvironment();
	void checkPlayerBehaviour(IslandApp &app);

	void spawnPlayer();

	void checkGuiOperations(EquipmentType type, sf::Vector2u field);

	void drawConsole(IslandApp &app);

	void updateTile(sf::Vector2u tileIndex);

	void drawTile(TerrainType &preTile, sf::Vector2u tileIndex, sf::RenderWindow &window, sf::RectangleShape &shp);
	void drawObject(size_t &preObjectId, sf::Vector2u objectIndex, sf::RenderWindow &window, sf::RectangleShape &shp);

	void drawWorld(IslandApp &app);
	void drawLyingItems(IslandApp &app);

	void drawPlayerGui(IslandApp & app);
	void pushItemTextureToRect(sf::Vector2f pos, unsigned itemId,sf::RectangleShape &rect);

public:
	Engine(GameVars &v1, unsigned MaxTileDrawRange);
	Engine(const Engine &other) = delete;
	~Engine();

	void operator()(IslandApp &app,char key,mouseWheel last,bool isMouseClick);

	void manageConsole(sf::Event &event, sf::Vector2f mousePos, bool isMouseRClick);

	void DrawAll(IslandApp &app);
};