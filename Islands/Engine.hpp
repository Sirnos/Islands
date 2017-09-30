#pragma once

#include <SFML/Graphics.hpp>

#include "IslandApp.hpp"

#include "WorldManager.hpp"
#include "MonsterManager.hpp"

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
	MonsterManager GMonsterManager;

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
	void checkPlayerBehaviour();
	void checkGuiOperations(const EquipmentType &type, const sf::Vector2u &field);


	void drawConsole(IslandApp &app);


	void updateTile(const sf::Vector2u &tileIndex);


	void drawMonsters(sf::RenderWindow &window);
	void drawTile(TerrainType &preTile, const sf::Vector2u &tileIndex, sf::RenderWindow &window, sf::RectangleShape &shp);
	void drawObject(size_t &preObjectId, const sf::Vector2u &objectIndex, sf::RenderWindow &window, sf::RectangleShape &shp);


	void drawWorld(IslandApp &app);
	void drawLyingItems(IslandApp &app);


	void drawPlayerGui(IslandApp & app);
	void pushItemTextureToRect(const sf::Vector2f &pos, unsigned itemId, sf::RectangleShape &rect);

public:
	Engine(GameVars &v1, unsigned MaxTileDrawRange);
	Engine(const Engine &other) = delete;
	~Engine();


	void operator()(IslandApp &app, char key, mouseWheel last, bool isMouseClick);


	void manageConsole(sf::Event &event, const sf::Vector2f &mousePos, bool isMouseRClick);


	void DrawAll(IslandApp &app);

};