#pragma once

#include <SFML/Graphics.hpp>

#include "IslandApp.hpp"

#include "WorldManager.hpp"
#include "MonsterManager.hpp"
#include "SavesManager.hpp"

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
	GameVars GameRules;
	RenderVars RenderRules;


	WorldManager GWorldManager;
	MonsterManager GMonsterManager;

	std::shared_ptr<World> GameWorld;
	GameComponents Components;


	Media mediaContainer;
	Gui GameGui;


	PlayerEntity Player;
	CraftingSystem Crafting;
	LyingItemsContainer LyingItems;


	sf::Clock GameClock;
	sf::Time lastUpdateLocalMapTime;


	Console GameConsole;


	void loadGameComponents();


	void checkPlayerEnvironment();
	void checkPlayerBehaviour();
	void checkGuiOperations(const EquipmentType &type, const sf::Vector2u &field);


	void drawConsole(IslandApp &app);


	void drawMonsters(sf::RenderWindow &window);
	void drawTile(TerrainType &preTile, const sf::Vector2u &tileIndex, sf::RenderWindow &window, sf::RectangleShape &shp);
	void drawObject(size_t &preObjectId, const sf::Vector2u &objectIndex, sf::RenderWindow &window, sf::RectangleShape &shp);


	void drawWorld(IslandApp &app);
	void drawLyingItems(IslandApp &app);


	void drawPlayerGui(IslandApp & app);
	void pushItemTextureToRect(const sf::Vector2f &pos, unsigned itemId, sf::RectangleShape &rect);

public:
	Engine(const GameVars &game, const RenderVars &render);
	Engine(const Engine &other) = delete;
	~Engine();


	void operator()(IslandApp &app, char key, mouseWheel last, bool isMouseClickL, bool isMouseClickR);


	void manageConsole(const sf::Event &event, const sf::Vector2f &mousePos, bool isMouseLClick);


	void DrawAll(IslandApp &app);

};