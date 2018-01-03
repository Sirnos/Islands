#include "Engine.hpp"

void Engine::loadGameComponents()
{
	GameComponentsLoader CoreLoader("Core", Components);
	CoreLoader.load();

	mediaContainer.pushTextures(TextureContainer::ObjectTextures, CoreLoader.getObjectsTextures().fromFile, CoreLoader.getObjectsTextures().TexturesCoords);
	mediaContainer.pushTextures(TextureContainer::ItemsTextures, CoreLoader.getObjectsTextures().fromFile, CoreLoader.getObjectsTextures().TexturesCoords);
	mediaContainer.pushTextures(TextureContainer::ItemsTextures, CoreLoader.getItemsTextures().fromFile, CoreLoader.getItemsTextures().TexturesCoords);


	std::vector<Structure> Structures = makeFromDef::makeStructure(CoreLoader.getStructures(), *Components.getObjects());
	std::vector<BiomeValuesDef> MapsDef(1);
	MapsDef.insert(MapsDef.end(), CoreLoader.getBiomes().begin(), CoreLoader.getBiomes().end());
	std::vector<BiomeValues> MapsVars = makeFromDef::makeBiome(MapsDef, *Components.getObjects(), Structures);
	GWorldManager.AssingStructures(Structures);
	GWorldManager.AssingLocalMapsBuilderVars(MapsVars);

	for (const auto & it : CoreLoader.getRecipes())
	{
		if (it.first == "Player")
		{
			Crafting.loadPlayerRecipes(makeFromDef::makeRecipes(it.second, *Components.getItems()));
			Crafting.usePlayerRecipes();
			break;
		}
	}
}

void Engine::checkPlayerEnvironment()
{
	sf::Vector2i PlayerCollectRectPos{ static_cast<sf::Vector2i>(Player.getCharacterCenterPosition()) - 
		static_cast<sf::Vector2i>((LyingItems.getLyingItemsPickUpRange() / 2.0f)) };
	sf::Vector2i PlayerColletRectSize{ static_cast<sf::Vector2i>(LyingItems.getLyingItemsPickUpRange()) };
	sf::FloatRect PlayerCollectRect{ static_cast<sf::Vector2f>(PlayerCollectRectPos),static_cast<sf::Vector2f>(PlayerColletRectSize) };

	ItemDefContainer *Items = Components.getItems().get();

	for (size_t i = 0; i < LyingItems.getSize(); i++)
	{
		if (PlayerCollectRect.contains(LyingItems.getPosition(i)))
		{
			ItemField temp = LyingItems.getItem(i);
			Player.Inventory.pushItem(temp, Items->getDefinition(temp.Id)->getMaxStack());
			if (temp.isEmpty())
			{
				LyingItems.eraseItem(i);
			}
			else
			{
				LyingItems.setItemAmount(i, temp.Amount);
			}
		}
	}
}

void Engine::checkPlayerBehaviour()
{
	sf::Vector2f movevctr;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		movevctr.x -= Player.Stats.Speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movevctr.x += Player.Stats.Speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		movevctr.y -= Player.Stats.Speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movevctr.y += Player.Stats.Speed;
	}

	if (movevctr != sf::Vector2f())
	{
		Player.Inventory.popInteractionWithChest();
		GameGui.deleteChestFields();
		Player.pushTexture(mediaContainer.getTexture(TextureContainer::EntitiesTextures, 1, Player.getActualEntitySide()));

		if (!Crafting.isUsedPlayerRecipes())
		{
			Crafting.clear();
			Crafting.usePlayerRecipes();
		}
	}

	Player.move(movevctr);

	sf::Vector2f PlayerPos = Player.getCharacterCenterPosition();
	sf::Vector2i tilePlayerPosition = World::getTiledPosition(PlayerPos);
	if (GWorldManager.getLocalMapTileCollisionBox(static_cast<sf::Vector2u>(tilePlayerPosition))
		.intersects(sf::IntRect(static_cast<sf::Vector2i>(Player.getBody().getPosition()), static_cast<sf::Vector2i>(Player.getBody().getSize()))))
	{
		Player.move(-movevctr);
	}
}

void Engine::checkGuiOperations(const Gui::EquipmentType &type, const sf::Vector2u &field)
{
	using namespace Gui;

	unsigned holdedItemId = Player.Inventory.getHoldItem().Id;
	ItemDefContainer *Items = Components.getItems().get();

	switch (type)
	{
	case EquipmentType::Inventory:
		if (holdedItemId != 0)
		{
			if (Player.Inventory.getInventoryField(field).Id == holdedItemId)
			{
				Player.Inventory.setInventoryField(field, ItemField(holdedItemId,
					Player.Inventory.getInventoryField(field).Amount + Player.Inventory.getHoldItem().Amount));

				unsigned maxStack = Items->getDefinition(holdedItemId)->getMaxStack();
				if (Player.Inventory.getInventoryField(field).Amount >= maxStack)
				{
					unsigned amountForHold = Player.Inventory.getInventoryField(field).Amount - maxStack;
					Player.Inventory.setInventoryField(field, ItemField(holdedItemId,
						Player.Inventory.getInventoryField(field).Amount - amountForHold));

					Player.Inventory.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					Player.Inventory.setHoldItem(ItemField(0,0));
				}

			}
			else if(Player.Inventory.getInventoryField(field).Id != 0)
			{
				ItemField temp(Player.Inventory.getInventoryField(field));
				Player.Inventory.setInventoryField(field, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(temp);
			}
			else
			{
				Player.Inventory.setInventoryField(field, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (Player.Inventory.getInventoryField(field).Id != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getInventoryField(field));
				Player.Inventory.setInventoryField(field, ItemField());
			}
		}
		break;
	case EquipmentType::Armor:
		if (holdedItemId != 0)
		{
			if (Player.Inventory.getArmorInventoryField(field.x).Id != 0)
			{
				ItemField temp(Player.Inventory.getArmorInventoryField(field.x));
				Player.Inventory.setArmorField(field.x, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(temp);
			}
			else
			{
				Player.Inventory.setArmorField(field.x, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (Player.Inventory.getArmorInventoryField(field.x).Id != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getArmorInventoryField(field.x));
				Player.Inventory.setArmorField(field.x, ItemField());
			}
		}
		break;
	case EquipmentType::Belt:
		if (holdedItemId != 0)
		{
			if (Player.Inventory.getHandInventoryField(field.x).Id == holdedItemId)
			{
				Player.Inventory.setHandInventoryField(field.x, ItemField(holdedItemId,
					Player.Inventory.getHandInventoryField(field.x).Amount + Player.Inventory.getHoldItem().Amount));

				unsigned maxStack = Items->getDefinition(holdedItemId)->getMaxStack();
				if (Player.Inventory.getHandInventoryField(field.x).Amount >= maxStack)
				{
					unsigned amountForHold = Player.Inventory.getHandInventoryField(field.x).Amount - maxStack;
					Player.Inventory.setHandInventoryField(field.x, ItemField(holdedItemId,
						Player.Inventory.getHandInventoryField(field.x).Amount - amountForHold));

					Player.Inventory.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					Player.Inventory.setHoldItem(ItemField(0, 0));
				}

			}
			else if (Player.Inventory.getHandInventoryField(field.x).Id != 0)
			{
				ItemField temp(Player.Inventory.getHandInventoryField(field.x));
				Player.Inventory.setHandInventoryField(field.x, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(temp);
			}
			else
			{
				Player.Inventory.setHandInventoryField(field.x, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (Player.Inventory.getHandInventoryField(field.x).Id != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getHandInventoryField(field.x));
				Player.Inventory.setHandInventoryField(field.x, ItemField());
			}
		}
		break;
	case EquipmentType::Chest:
		if (holdedItemId != 0)
		{
			if (holdedItemId == Player.Inventory.getItemFromInteractedChest(field.x).Id)
			{
				Player.Inventory.setInteractedChestItemField(field.x, ItemField(holdedItemId,
					Player.Inventory.getItemFromInteractedChest(field.x).Amount + Player.Inventory.getHoldItem().Amount));

				unsigned maxStack = Items->getDefinition(holdedItemId)->getMaxStack();
				if (Player.Inventory.getItemFromInteractedChest(field.x).Amount >= maxStack)
				{
					unsigned amountForHold = Player.Inventory.getInventoryField(field).Amount - maxStack;
					Player.Inventory.setInteractedChestItemField(field.x, ItemField(holdedItemId,
						Player.Inventory.getItemFromInteractedChest(field.x).Amount - amountForHold));

					Player.Inventory.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					Player.Inventory.setHoldItem(ItemField(0, 0));
				}
			}
			else if(Player.Inventory.getItemFromInteractedChest(field.x).Id != 0)
			{

				ItemField temp(Player.Inventory.getItemFromInteractedChest(field.x));
				Player.Inventory.setInteractedChestItemField(field.x, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(temp);
			}
			else
			{
				Player.Inventory.setInteractedChestItemField(field.x, Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (Player.Inventory.getItemFromInteractedChest(field.x).Id != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getItemFromInteractedChest(field.x));
				Player.Inventory.setInteractedChestItemField(field.x, ItemField());
			}
		}
	default:
		break;
	}
}

void Engine::drawConsole(IslandApp & app)
{
	if (!GameConsole.getEnable()) { return; }
	app.draw(GameConsole.getWindow());
	app.draw(GameConsole.getInputText());

	if (GameConsole.getCommandsSize() > 0)
	{
		sf::Text consoleText;
		sf::Font consoleTextFont;
		consoleTextFont.loadFromFile("Data/Fonts/ariali.ttf");

		consoleText.setCharacterSize(16);
		consoleText.setFont(consoleTextFont);
		consoleText.setFillColor(sf::Color::White);

		sf::Vector2f begPos = GameConsole.getInputText().getPosition();
		begPos -= sf::Vector2f(0, 16);


		if (GameConsole.getCommandsSize() == 0) { return; }
		for (size_t i = GameConsole.getCommandsSize() - 1; i != -1; i--)
		{
			if (begPos.y < GameConsole.getWindow().getPosition().y) { break; }
			consoleText.setString(GameConsole.getText(i));
			consoleText.setPosition(begPos);
			app.draw(consoleText);
			begPos -= sf::Vector2f(0, 16);
		}
	}
}

void Engine::manageConsole(const sf::Event &event, const sf::Vector2f &mousePos, bool isMouseLClick)
{
	if (!GameConsole.getEnable()) { return; }

	auto tmp = GameConsole(event, mousePos, isMouseLClick);

	if (!GameConsole.getTextboxEnable()) { return; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (GameConsole.getHistorySize() > 0)
		{
			if (GameConsole.getLastHistoryCmdNumber() == 0)
			{
				GameConsole.getLastHistoryCmdNumber() = GameConsole.getHistorySize() - 1;
				GameConsole.setCurrentText(GameConsole.getLastCmdFromHistory());
			}
			else
			{
				GameConsole.setCurrentText(GameConsole.getCmdFromHistory(GameConsole.getLastHistoryCmdNumber()));
				GameConsole.getLastHistoryCmdNumber() -= 1;
			}
		}
	}

	if (tmp.size() > 0 )
	{
		if (tmp[0] == '/')
		{
			if (tmp == "/help")
			{
				GameConsole.pushText(std::string("Commands:/giveItem,/spawnmonster"));
				GameConsole.pushText(std::string("/placeObject,/settile,/settime"));
				GameConsole.pushText(std::string("/Playerposition,/time,/worldsize"));
				GameConsole.pushText(std::string("/clear,/"));

				GameConsole.pushCommandToHistory(tmp);
			}
			else if(tmp == "/Playerposition")
			{
				GameConsole.pushText(std::string("Pos-x: ") + std::to_string(Player.getCharacterCenterPosition().x));
				GameConsole.pushText(std::string("Pos-y: ") + std::to_string(Player.getCharacterCenterPosition().y));
				GameConsole.pushCommandToHistory(tmp);
			}
			else if(tmp == "/time")
			{
				GameConsole.pushText(std::string("Time: ") + std::to_string(GameClock.getElapsedTime().asSeconds()));
				GameConsole.pushCommandToHistory(tmp);
			}
			else if(tmp == "/clear")
			{
				GameConsole.clearConsole();
				GameConsole.pushCommandToHistory(tmp);
				GameConsole.clearHistory();
				GameConsole.getLastHistoryCmdNumber() = 0;
			}
			else if(tmp == "/worldsize")
			{
				GameConsole.pushText(std::string("Size: ") + std::to_string(GameWorld->getLocalMapSize()));
				GameConsole.pushCommandToHistory(tmp);
			}
			else if(tmp.find("/giveItem") != std::string::npos)
			{
				GameConsole.giveItemCheck(tmp, *Components.getItems().get(), Player.Inventory);
			}
			else
			{
				GameConsole.pushText(std::string("Unspecified command"));
			}
		}
		else
		{
			GameConsole.pushText(tmp);
		}
	}
}

void Engine::drawMonsters(sf::RenderWindow & window)
{
	sf::FloatRect cameraPos{ window.getView().getCenter() - (window.getView().getSize() / 2.0f),window.getView().getSize() };

	for (const auto & monster : GMonsterManager.getMonsters())
	{
		if (cameraPos.contains(monster.getCharacterCenterPosition()))
		{
			window.draw(monster.getBody());
		}
	}
}

void Engine::drawTile(TerrainType &preTile, const sf::Vector2u &tileIndex, sf::RenderWindow & window, sf::RectangleShape &shp)
{
	TerrainType TerrainType = GameWorld->getLocalMapTileTerrain(tileIndex);
	if (TerrainType == TerrainType::Null) { return; }
	shp.setPosition(sf::Vector2f(World::getNormalPosition(static_cast<sf::Vector2i>(tileIndex))));
	if (preTile != TerrainType)
	{
		shp.setTexture(mediaContainer.getTexture(TextureContainer::TerrainTextures, static_cast<size_t>(TerrainType)));
	}

	preTile = TerrainType;
	window.draw(shp);
}

void Engine::drawObject(size_t &preObjectId, const sf::Vector2u &objectIndex, sf::RenderWindow & window, sf::RectangleShape &shp)
{
	unsigned ObjectID = GameWorld->getLocalMapTileObjectId(objectIndex);
	if (ObjectID == 0) { return; }
	if (ObjectID > Components.getObjects().get()->getSize()) { return; }

	shp.setPosition(sf::Vector2f(World::getNormalPosition(static_cast<sf::Vector2i>(objectIndex))));
	bool isTree = false;
	isTree = (Components.getObjects()->getDefinition(ObjectID)->getType() == ObjectType::Tree);

	if (preObjectId != ObjectID)
	{
		shp.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		shp.setTexture(mediaContainer.getTexture(TextureContainer::ObjectTextures, ObjectID), true);

		if (isTree)
		{
			shp.move(-TILE_SIZE, -(3.0f * TILE_SIZE));
			shp.setSize(sf::Vector2f(TILE_SIZE * 3.0f, TILE_SIZE * 4.0f));
		}
	}
	else
	{
		if (isTree)
		{
			shp.move(-TILE_SIZE, -(3.0f * TILE_SIZE));
		}
	}

	window.draw(shp);
	preObjectId = ObjectID;
}

Engine::Engine(const GameVars &game, const RenderVars &render , const std::string &saveName)
	:Player(sf::RectangleShape{ sf::Vector2f(48,64) }, EntityStats(DEFAULT_PLAYER_HP, DEFAULT_PLAYER_MP, DEFAULT_PLAYER_SPEED)),
	GameRules(game), RenderRules(render), GameWorld(new World), GSavesManager(saveName),
	GameConsole(sf::Vector2f(400.0f, 600.0f), sf::Color(36, 10, 92, 120), 16)
{
	if (!GSavesManager.isValid())
	{
		ErrorHandler::logToFile("Cannot open databases \n");
	}

	std::vector<sf::IntRect> terrainTextureCords;
	GameComponentsLoader::loadTerrainTextureCoords(terrainTextureCords);
	mediaContainer.pushTextures(TextureContainer::TerrainTextures, boost::filesystem::current_path().string() + SETTINGS_DIR.string() + "Terrain.png", terrainTextureCords);

	Components.getObjects().get()->getContainer()[0] = new ObjectDef;
	Components.getItems().get()->getContainer()[0] = new PlaceableDef("");

	ErrorHandler::clearLogFile();
	loadGameComponents();
	Crafting.AssingItemDef(Components.getItems());


	LyingItems.init(GameRules.MaxNumberOfLyingItems, static_cast<sf::Vector2f>(sf::Vector2u(GameRules.PlayerPickUpItemsRange, GameRules.PlayerPickUpItemsRange)));

	
	GWorldManager.setStructuresAmountInLocalMap(GameRules.StructuresPerLocalMap);
	GWorldManager.AssingClock(GameClock);
	GWorldManager.AssingItemsDef(Components.getItems());
	GWorldManager.AssingObjectsDef(Components.getObjects());
	GWorldManager.AssingWorld(GameWorld);
	if (!GWorldManager.buildLocalMap(GSavesManager.loadMapTerrain(), GSavesManager.loadMapObjects(), GameRules.LocalMapSize))
	{
		GWorldManager.buildLocalMap(TerrainType::Grass, GameRules.LocalMapSize);
	}


	Player.Stats = Components.getEntities()->getContainer().front().getStats();
	Player.pushTexture(mediaContainer.getTexture(TextureContainer::EntitiesTextures, 1));
	sf::Vector2f playerSpawnPos = GWorldManager.getSpawnPosition();

	ErrorHandler::log(std::string("Spawn Player position:"));
	ErrorHandler::log("Tile Y " + std::to_string(World::getTiledPosition(playerSpawnPos).y));
	ErrorHandler::log("Tile X " + std::to_string(World::getTiledPosition(playerSpawnPos).x));

	Player.setPosition(playerSpawnPos);
	Player.setSpawnPoint(playerSpawnPos);

	GMonsterManager.assingMonsterWorld(GameWorld);
	GMonsterManager.addEntityToObserved(&Player);

	GSavesManager.loadPlayerStats(Player);
	GSavesManager.loadPlayerInventory(Player.Inventory);
}

Engine::~Engine()
{
	GSavesManager.savePlayerStats(Player);
	GSavesManager.savePlayerInventory(Player.Inventory);
	GSavesManager.saveLocalMap(GameWorld->getLocalMap());


	ErrorHandler::log("Clear data");
}

void Engine::operator()(IslandApp &app, char key, mouseWheel last, bool isMouseClickL, bool isMouseClickR)
{
	using namespace Gui;
	LyingItems.clearOldItems(GameClock.getElapsedTime());

	if (GameClock.getElapsedTime() >= lastUpdateLocalMapTime + sf::seconds(GameRules.TimeToUpdateLocalMap))
	{
		lastUpdateLocalMapTime = GameClock.getElapsedTime();
		GWorldManager.updateTiles();
	}
	
	ObjectDefContainer *Objects = Components.getObjects().get();
	ItemDefContainer *Items = Components.getItems().get();

	checkPlayerBehaviour();
	checkPlayerEnvironment();

	app.getIslandWindow()->setView(*app.getIslandView());

	auto Window = app.getIslandWindow();
	GameConsole.setPosition(Window->mapPixelToCoords(sf::Vector2i(0, 0)));

	if (key == '`')
	{
		GameConsole.setEnable(!GameConsole.getEnable());
		GameConsole.setTexboxEnable(!GameConsole.getTextboxEnable());
	}

	if (isMouseClickL)
	{
		if (GameGui.Eq.isEnable)
		{
			sf::Vector2f mousePosInWorld = app.getMousePosInWorld();

			for (size_t i = 0; i < PLAYER_INVENTORY_SIZE; i++)
			{
				if (i < PLAYER_ARMOR_INVENTORY_SIZE)
				{
					if (GameGui.Eq.ArmorEquipment[i].isHover)
					{
						checkGuiOperations(EquipmentType::Armor, sf::Vector2u(i, 0));
					}
				}

				if (GameGui.Hud.Belt[i].isHover)
				{
					checkGuiOperations(EquipmentType::Belt, sf::Vector2u(i, 0));
				}

				for (size_t j = 0; j < PLAYER_INVENTORY_SIZE; j++)
				{
					if (GameGui.Eq.Equipment[i][j].isHover)
					{
						checkGuiOperations(EquipmentType::Inventory, sf::Vector2u(i, j));
					}
				}
			}
			if (Player.Inventory.isInteractedChestExist())
			{
				for (size_t i = 0; i < Player.Inventory.getInteractedChestSize(); i++)
				{
					if (GameGui.Chest.ChestContain[i].isHover)
					{
						checkGuiOperations(EquipmentType::Chest, sf::Vector2u(i, 0));
					}
				}
			}
		}
		else if (GameGui.Craft.isEnable)
		{
			size_t RecipeNumber = 0;
			for (auto & i : GameGui.Craft.RecipeFields)
			{
				for (auto & j : i)
				{
					if (j.isHover)
					{
						Crafting.setSelectedRecipe(RecipeNumber);
					}

					RecipeNumber++;
				}
			}

			sf::Vector2i mousePos = sf::Mouse::getPosition(*app.getIslandWindow());
			if (GameGui.Craft.RecipeInfo.CraftButton.isClick(mousePos))
			{
				ItemField craftedItem = Crafting.craftItemFromRecipe(Player.Inventory);
				if (!craftedItem.isEmpty())
				{
					Player.Inventory.pushItem(craftedItem, Items->getDefinition(craftedItem.Id)->getMaxStack());
					Crafting.clearPlayerSelects();
				}
			}
			if (GameGui.Craft.RecipeInfo.CraftAmountAddOne.isClick(mousePos))
			{
				Crafting.setCraftAmount(Crafting.getCraftAmount() + 1);
			}
			if (GameGui.Craft.RecipeInfo.CraftAmountSubOne.isClick(mousePos))
			{
				unsigned craftAmount = Crafting.getCraftAmount();
				if (craftAmount == 0 || craftAmount == -1)
				{
					Crafting.setCraftAmount(0);
				}
				else { Crafting.setCraftAmount(craftAmount - 1); }
			}
		}
		else if (!GameGui.Eq.isEnable && Player.Inventory.getHoldItem().isEmpty())
		{
			ItemField item = Player.Inventory.getHandInventoryField(GameGui.Hud.ActiveBeltField);
			sf::Vector2f mousePos = app.getMousePosInWorld();
			sf::Vector2u objectPos(static_cast<sf::Vector2u>(mousePos / TILE_SIZE));

			if (!item.isEmpty())
			{
				if (sf::FloatRect(sf::Vector2f(), sf::Vector2f(GameWorld->getLocalMapSize() * TILE_SIZE,
					GameWorld->getLocalMapSize() * TILE_SIZE)).contains(mousePos))
				{
					if (GWorldManager.placeObject(objectPos, item.Id))
					{
						item -= 1;
						Player.Inventory.setHandInventoryField(GameGui.Hud.ActiveBeltField, item);
					}
				}
			}
			else
			{
				unsigned objectId = GameWorld->getLocalMapTileObjectId(objectPos);
				if (objectId > 0)
				{
					sf::Time timeAtMouseClick = GameClock.getElapsedTime();
					sf::Time timeOfMouseClickHold;

					while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						timeOfMouseClickHold = GameClock.getElapsedTime();
						if (timeOfMouseClickHold.asMilliseconds() - timeAtMouseClick.asMilliseconds() >= 101)
						{
							for (const auto & i : Objects->getDefinition(objectId)->getYield())
							{
								sf::Time currentTime = GameClock.getElapsedTime();
								if (i.first != NULL_ITEM_YIELD)
								{
									if (i.first == SELF_ITEM_YIELD)
									{
										LyingItems.pushNewItem(currentTime, mousePos, ItemField(objectId, i.second));
										break;
									}
									else
									{
										ItemField item;
										item.Id = Items->getDefIdbyName(i.first);
										item.Amount = i.second;
										LyingItems.pushNewItem(currentTime, mousePos, item);
									}
								}
							}

							for (const auto & Item : GWorldManager.getItemsFromChestObject(objectPos))
							{
								LyingItems.pushNewItem(GameClock.getElapsedTime(), mousePos, Item);
							}
							GameWorld->removeLocalMapTileObject(objectPos);
							break;
						}
					}
				}
			}
		}
	}
	if(isMouseClickR)
	{
		sf::Vector2f mousePos = app.getMousePosInWorld();
		sf::Vector2u objectPos(static_cast<sf::Vector2u>(mousePos / TILE_SIZE));

		if (GameWorld->getLocalMapTileObject(objectPos) != nullptr)
		{
			ObjectType clickedObjectType = GameWorld->getLocalMapTileObject(objectPos)->getType();

			switch (clickedObjectType)
			{
			case ObjectType::Default:
				break;
			case ObjectType::Chest:
				Player.Inventory.pushInteractionWithChest(&dynamic_cast<ChestObject*>(GameWorld->getLocalMapTileObject(objectPos))->getContain());
				GameGui.createChestFields(Player.Inventory.getInteractedChestSize());
				GameGui.Eq.isEnable = true;
				break;
			case ObjectType::CraftingPlace:
				Crafting.clear();
				Crafting.AssingRecipes(dynamic_cast<CraftingPlaceObject*>(GameWorld->getLocalMapTileObject(objectPos))->getRecipes());
				GameGui.Craft.isEnable = true;
				break;
			case ObjectType::Tree:
				break;
			case ObjectType::Sapling:
				break;
			case ObjectType::Spawner:
				break;
			default:
				break;
			}
		}

		if (!GameGui.Eq.isEnable)
		{
			if (Player.Inventory.getHoldItem().Id != 0)
			{
				LyingItems.pushNewItem(GameClock.getElapsedTime(), app.getMousePosInWorld(), Player.Inventory.getHoldItem());
				Player.Inventory.setHoldItem(ItemField(0, 0));
			}
		}
	}

	switch (last)
	{
	case mouseWheel::Up:
		GameGui.incrActiveBeltField();
		break;
	case mouseWheel::Down:
		GameGui.decrActiveBeltField();
		break;
	case mouseWheel::Stop:
		break;
	default:
		break;
	}

	GameGui.pushKeyState(key);
	GameGui.Hud.pushNewValuesForHpInfo(static_cast<unsigned>(Player.Stats.HP.getLimit()), static_cast<unsigned>(Player.Stats.HP.getVar()));
	GameGui.Hud.pushNewValuesForMpInfo(static_cast<unsigned>(Player.Stats.MP.getLimit()), static_cast<unsigned>(Player.Stats.MP.getVar()));
}

void Engine::drawWorld(IslandApp & app)
{
	sf::Vector2i PlayerPosToTile = World::getTiledPosition(Player.getCharacterCenterPosition());
	sf::RectangleShape TileShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	sf::RectangleShape ObjectShape(TileShape);

	int MapSize = static_cast<int>(GameWorld->getLocalMapSize());
	TerrainType preTile = TerrainType::Null;
	size_t preObjectId = 0;

	sf::Vector2i begin(PlayerPosToTile.x - RenderRules.TileDrawRange, PlayerPosToTile.y - RenderRules.TileDrawRange);
	if (begin.y < 0)
	{
		begin.y = 0;
	}
	if (begin.x < 0)
	{
		begin.x = 0;
	}
	sf::Vector2i end(PlayerPosToTile.x + RenderRules.TileDrawRange + 1, PlayerPosToTile.y + RenderRules.TileDrawRange + 1);
	if (end.x > MapSize)
	{
		end.x = MapSize;
	}
	if (end.y > MapSize)
	{
		end.y = MapSize;
	}

	
	for (int y_tile = begin.y; y_tile < end.y; y_tile++)
	{
		for (int x_tile = begin.x; x_tile < end.x; x_tile++)
		{
			sf::Vector2u currentTile = static_cast<sf::Vector2u>(sf::Vector2i(x_tile, y_tile));

			drawTile(preTile, currentTile, *app.getIslandWindow(), TileShape);
			drawObject(preObjectId, currentTile, *app.getIslandWindow(), ObjectShape);
		}
	}
}

void Engine::drawLyingItems(IslandApp & app)
{
	if (LyingItems.getSize() == 0) { return; }

	sf::RectangleShape LyingItemShape(sf::Vector2f(32, 32));

	sf::Vector2f cameraPos{ app.getIslandView()->getCenter() - sf::Vector2f(app.getIslandView()->getSize() / 2.0f) };
	sf::Vector2f cameraSize{ app.getIslandView()->getSize() };
	sf::FloatRect cameraRange{ cameraPos,cameraSize };

	for (size_t i = 0; i < LyingItems.getSize(); i++)
	{
		if (cameraRange.contains(LyingItems.getPosition(i)))
		{
			LyingItemShape.setPosition(LyingItems.getPosition(i));
			LyingItemShape.setTexture(mediaContainer.getTexture(TextureContainer::ItemsTextures,
				LyingItems.getItem(i).Id), true);
			app.draw(LyingItemShape);
		}
	}
}

void Engine::drawPlayerGui(IslandApp & app)
{
	GameGui.Hud.HpInfo.setPosition(app.getIslandWindow()->mapPixelToCoords(GameGui.Hud.HpInfoScreenPos));
	GameGui.Hud.MpInfo.setPosition(app.getIslandWindow()->mapPixelToCoords(GameGui.Hud.MpInfoScreenPos));
	app.draw(GameGui.Hud.HpInfo);
	app.draw(GameGui.Hud.MpInfo);

	sf::Text amountItem("", GameGui.GuiFont, 16);
	amountItem.setFillColor(sf::Color(255, 0, 0, 255));

	sf::RectangleShape FieldShape(sf::Vector2f(EquipmentFieldSize, EquipmentFieldSize));
	sf::RectangleShape TextureFieldShape(FieldShape);
	sf::Vector2f mousePosition = app.getMousePosInWorld();

	if (!Player.Inventory.getHoldItem().isEmpty())
	{
		sf::RectangleShape holdItemRep;
		holdItemRep.setSize(sf::Vector2f(EquipmentFieldSize, EquipmentFieldSize));
		holdItemRep.setTexture(mediaContainer.getTexture(TextureContainer::ItemsTextures,
			Player.Inventory.getHoldItem().Id));
		holdItemRep.setPosition(app.getMousePosInWorld());

		app.draw(holdItemRep);
	}


	auto checkEquipmentField = [&app, mousePosition, &FieldShape](EquipmentFieldInfo &field, sf::Vector2f &newFieldPos)
	{
		field.checkIsHover(mousePosition);
		newFieldPos = app.getIslandWindow()->mapPixelToCoords(field.ScreenPosition);
		field.Position = newFieldPos;
		field.pushChangesToRectangleShape(FieldShape);
	};
	auto drawShapes = [&app](sf::RectangleShape &field, sf::RectangleShape &texturedField, sf::Text &text, bool isArmorInv = false)
	{
		app.draw(field);
		if (texturedField.getTexture() != nullptr) { app.draw(texturedField); }
		if (isArmorInv) { return; }
		if (!text.getString().isEmpty()) { app.draw(text); }
	};


	for (size_t i = 0; i < PLAYER_INVENTORY_SIZE; i++)
	{
		amountItem.setString("");
		sf::Vector2f newPos;

		checkEquipmentField(GameGui.Hud.Belt[i], newPos);

		ItemField temp = Player.Inventory.getHandInventoryField(i);
		pushItemTextureToRect(newPos, temp.Id, TextureFieldShape);
		amountItem.setPosition(FieldShape.getPosition());

		if (temp.Id != 0) { amountItem.setString(std::to_string(temp.Amount)); }
		if (i == GameGui.Hud.ActiveBeltField) { FieldShape.setFillColor(ActiveBeltFieldColor); }

		drawShapes(FieldShape, TextureFieldShape, amountItem);
	}

	if (GameGui.Eq.isEnable && !GameGui.Craft.isEnable)
	{
		sf::Vector2u field;
		for (field.x = 0; field.x < PLAYER_INVENTORY_SIZE; field.x++)
		{
			for (field.y = 0; field.y < PLAYER_INVENTORY_SIZE; field.y++)
			{
				if (field.x < PLAYER_ARMOR_INVENTORY_SIZE)
				{
					sf::Vector2f newPos;
					checkEquipmentField(GameGui.Eq.ArmorEquipment[field.x], newPos);

					ItemField temp = Player.Inventory.getArmorInventoryField(field.x);
					pushItemTextureToRect(newPos, temp.Id, TextureFieldShape);

					drawShapes(FieldShape, TextureFieldShape, amountItem, true);
				}

				amountItem.setString("");
				sf::Vector2f newPos;

				checkEquipmentField(GameGui.Eq.Equipment[field.x][field.y], newPos);

				ItemField temp = Player.Inventory.getInventoryField(field);
				pushItemTextureToRect(newPos, temp.Id, TextureFieldShape);
				amountItem.setPosition(newPos);

				if (temp.Amount != 0) { amountItem.setString(std::to_string(temp.Amount)); }

				drawShapes(FieldShape, TextureFieldShape, amountItem);
			}
		}
	}

	if (Player.Inventory.isInteractedChestExist() && GameGui.Eq.isEnable)
	{
		for (size_t i = 0; i < Player.Inventory.getInteractedChestSize(); i++)
		{
			sf::Vector2f newPos;
			checkEquipmentField(GameGui.Chest.ChestContain[i], newPos);

			ItemField temp = Player.Inventory.getItemFromInteractedChest(i);
			pushItemTextureToRect(newPos, temp.Id, TextureFieldShape);
			amountItem.setPosition(newPos);

			if (temp.Amount != 0) { amountItem.setString(std::to_string(temp.Amount)); }
			
			drawShapes(FieldShape, TextureFieldShape, amountItem);
			amountItem.setString("");
		}
	}

	if (GameGui.Craft.isEnable)
	{
		size_t RecipeNumber = 0;
		for (auto & i : GameGui.Craft.RecipeFields)
		{
			for (auto & j : i)
			{
				sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords(j.ScreenPosition);
				j.Position = newPos;

				j.checkIsHover(mousePosition);
				j.pushChangesToRectangleShape(FieldShape);
				
				if (RecipeNumber == Crafting.getSelectedRecipeNumber())
				{
					FieldShape.setFillColor(HoverEquipmentFieldColor);
				}

				app.draw(FieldShape);
				if (RecipeNumber < Crafting.getRecipesNumber())
				{
					ItemField temp = Crafting.getRecipeOut(RecipeNumber);
					pushItemTextureToRect(newPos, temp.Id, TextureFieldShape);
					if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
					if (!temp.isEmpty())
					{
						amountItem.setPosition(newPos);
						amountItem.setString(std::to_string(temp.Amount));
						app.draw(amountItem);
						amountItem.setString("");
					}
				}
				RecipeNumber++;
			}
		}
		GameGui.Craft.RecipeInfo.Window.setPosition(app.getIslandWindow()->mapPixelToCoords(GameGui.Craft.RecipeInfo.WindowScreenPosition));
		app.draw(GameGui.Craft.RecipeInfo.Window);
		GameGui.Craft.RecipeInfo.CraftButton.setButtonPosition(
			app.getIslandWindow()->mapPixelToCoords(GameGui.Craft.RecipeInfo.CraftButton.getInterBoxPos()));
		GameGui.Craft.RecipeInfo.CraftAmountAddOne.setButtonPosition(
			app.getIslandWindow()->mapPixelToCoords(GameGui.Craft.RecipeInfo.CraftAmountAddOne.getInterBoxPos()));
		GameGui.Craft.RecipeInfo.CraftAmountSubOne.setButtonPosition(
			app.getIslandWindow()->mapPixelToCoords(GameGui.Craft.RecipeInfo.CraftAmountSubOne.getInterBoxPos()));
		GameGui.Craft.RecipeInfo.CraftAmountInfo.setString(std::to_string(Crafting.getCraftAmount()));
		GameGui.Craft.RecipeInfo.CraftAmountInfo.setPosition(
			app.getIslandWindow()->mapPixelToCoords(GameGui.Craft.RecipeInfo.CraftamountInfoScreenPos));
		app.draw(GameGui.Craft.RecipeInfo.CraftButton.getText());
		app.draw(GameGui.Craft.RecipeInfo.CraftAmountAddOne.getText());
		app.draw(GameGui.Craft.RecipeInfo.CraftAmountSubOne.getText());
		app.draw(GameGui.Craft.RecipeInfo.CraftAmountInfo);
	}
}

void Engine::pushItemTextureToRect(const sf::Vector2f &pos, unsigned itemId, sf::RectangleShape & rect)
{
	rect.setPosition(pos);
	if (itemId > 0) { rect.setTexture(mediaContainer.getTexture(TextureContainer::ItemsTextures, itemId), true); }
	else { rect.setTexture(nullptr); }
}

void Engine::DrawAll(IslandApp &app)
{
	app.getIslandView()->setCenter(Player.getCharacterCenterPosition());

	drawWorld(app);
	drawLyingItems(app);
	drawMonsters(*app.getIslandWindow());
	app.draw(Player.getBody());

	drawPlayerGui(app);
	drawConsole(app);
}