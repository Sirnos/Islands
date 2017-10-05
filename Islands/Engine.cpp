#include "Engine.hpp"

void Engine::loadGameComponents()
{
	std::string objectGraphicsfile;
	std::vector<sf::IntRect> objectTextureCords;

	sf::Clock ObjectsDefLoadClock;
	GameComponentsLoader::LoadObjectDefFromFile(Objects->getContainer(), objectGraphicsfile, objectTextureCords);
	mediaContainer.pushTextures(TextureContainer::ObjectTextures, objectGraphicsfile, objectTextureCords);
	mediaContainer.pushTextures(TextureContainer::ItemsTextures, objectGraphicsfile, objectTextureCords);

	ErrorHandler::logToFile("Load Objects Definitions [Size] = " + std::to_string(Objects->getSize()) + 
		" [Time] = " + std::to_string(ObjectsDefLoadClock.getElapsedTime().asMilliseconds()) + " milisecs");

	sf::Clock ItemsDefLoadClock;
	GameComponentsLoader::GenerateItemsFromObjectDef(Objects->getContainer(), Items->getContainer());

	std::string itemGraphicsFile;
	std::vector<sf::IntRect> itemTextureCords;
	GameComponentsLoader::LoadItemDefFromFile(Items->getContainer(), itemGraphicsFile, itemTextureCords);
	mediaContainer.pushTextures(TextureContainer::ItemsTextures, itemGraphicsFile, itemTextureCords);


	ErrorHandler::logToFile("Load Items Definitions [Size] = " + std::to_string(Items->getSize()) + 
		" [Time] = " + std::to_string(ItemsDefLoadClock.getElapsedTime().asMilliseconds()) + " milisecs");

	std::vector<sf::Rect<int>> terrainTextureCords;
	GameComponentsLoader::LoadTerrainTextureCoords(terrainTextureCords);
	mediaContainer.pushTextures(TextureContainer::TerrainTextures, objectGraphicsfile, terrainTextureCords);

	sf::Clock entitiesDefLoadClock;
	GameComponentsLoader::LoadEntitiesDefFromFile(Entities.get()->getContainer());

	ErrorHandler::logToFile("Load Entities Definitions [Size] = " + std::to_string(Entities->getSize()) +
		" [Time] = " + std::to_string(entitiesDefLoadClock.getElapsedTime().asMilliseconds()) + " milisecs");
}

void Engine::checkPlayerEnvironment()
{
	sf::Vector2i PlayerCollectRectPos{ static_cast<sf::Vector2i>(Player.getCharacterCenterPosition()) - 
		static_cast<sf::Vector2i>((LyingItems.getLyingItemsPickUpRange() / 2.0f)) };
	sf::Vector2i PlayerColletRectSize{ static_cast<sf::Vector2i>(LyingItems.getLyingItemsPickUpRange()) };

	sf::FloatRect PlayerCollectRect{ static_cast<sf::Vector2f>(PlayerCollectRectPos),static_cast<sf::Vector2f>(PlayerColletRectSize) };

	for (size_t i = 0; i < LyingItems.getSize(); i++)
	{
		if (PlayerCollectRect.contains(LyingItems.getPosition(i)))
		{
			ItemField temp = LyingItems.getItem(i);
			Player.Inventory.pushItem(temp, Items->getDefinition(temp.ItemId)->getMaxStack());
			if (temp.isEmpty())
			{
				LyingItems.eraseItem(i);
			}
			else
			{
				LyingItems.setItemAmount(i, temp.ItemAmount);
			}
		}
	}
}

void Engine::checkPlayerBehaviour()
{
	sf::Vector2f movevctr;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movevctr.x -= Player.Stats.Speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movevctr.x += Player.Stats.Speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movevctr.y -= Player.Stats.Speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
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

void Engine::checkGuiOperations(const EquipmentType &type, const sf::Vector2u &field)
{
	unsigned holdedItemId = Player.Inventory.getHoldItem().ItemId;
	switch (type)
	{
	case EquipmentType::Inventory:
		if (holdedItemId != 0)
		{
			if (Player.Inventory.getInventoryField(field).ItemId == holdedItemId)
			{
				Player.Inventory.setInventoryField(field, ItemField(holdedItemId,
					Player.Inventory.getInventoryField(field).ItemAmount + Player.Inventory.getHoldItem().ItemAmount));

				unsigned maxStack = Items->getDefinition(holdedItemId)->getMaxStack();
				if (Player.Inventory.getInventoryField(field).ItemAmount >= maxStack)
				{
					unsigned amountForHold = Player.Inventory.getInventoryField(field).ItemAmount - maxStack;
					Player.Inventory.setInventoryField(field, ItemField(holdedItemId,
						Player.Inventory.getInventoryField(field).ItemAmount - amountForHold));

					Player.Inventory.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					Player.Inventory.setHoldItem(ItemField(0,0));
				}

			}
			else if(Player.Inventory.getInventoryField(field).ItemId != 0)
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
			if (Player.Inventory.getInventoryField(field).ItemId != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getInventoryField(field));
				Player.Inventory.setInventoryField(field, ItemField());
			}
		}
		break;
	case EquipmentType::Armor:
		if (holdedItemId != 0)
		{
			if (Player.Inventory.getArmorInventoryField(field.x).ItemId != 0)
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
			if (Player.Inventory.getArmorInventoryField(field.x).ItemId != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getArmorInventoryField(field.x));
				Player.Inventory.setArmorField(field.x, ItemField());
			}
		}
		break;
	case EquipmentType::Belt:
		if (holdedItemId != 0)
		{
			if (Player.Inventory.getHandInventoryField(field.x).ItemId == holdedItemId)
			{
				Player.Inventory.setHandInventoryField(field.x, ItemField(holdedItemId,
					Player.Inventory.getHandInventoryField(field.x).ItemAmount + Player.Inventory.getHoldItem().ItemAmount));

				unsigned maxStack = Items->getDefinition(holdedItemId)->getMaxStack();
				if (Player.Inventory.getHandInventoryField(field.x).ItemAmount >= maxStack)
				{
					unsigned amountForHold = Player.Inventory.getHandInventoryField(field.x).ItemAmount - maxStack;
					Player.Inventory.setHandInventoryField(field.x, ItemField(holdedItemId,
						Player.Inventory.getHandInventoryField(field.x).ItemAmount - amountForHold));

					Player.Inventory.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					Player.Inventory.setHoldItem(ItemField(0, 0));
				}

			}
			else if (Player.Inventory.getHandInventoryField(field.x).ItemId != 0)
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
			if (Player.Inventory.getHandInventoryField(field.x).ItemId != 0)
			{
				Player.Inventory.setHoldItem(Player.Inventory.getHandInventoryField(field.x));
				Player.Inventory.setHandInventoryField(field.x, ItemField());
			}
		}
		break;
	case EquipmentType::Chest:
		if (holdedItemId != 0)
		{
			if (holdedItemId == Player.Inventory.getItemFromInteractedChest(field.x).ItemId)
			{
				Player.Inventory.setInteractedChestItemField(field.x, ItemField(holdedItemId,
					Player.Inventory.getItemFromInteractedChest(field.x).ItemAmount + Player.Inventory.getHoldItem().ItemAmount));

				unsigned maxStack = Items->getDefinition(holdedItemId)->getMaxStack();
				if (Player.Inventory.getItemFromInteractedChest(field.x).ItemAmount >= maxStack)
				{
					unsigned amountForHold = Player.Inventory.getInventoryField(field).ItemAmount - maxStack;
					Player.Inventory.setInteractedChestItemField(field.x, ItemField(holdedItemId,
						Player.Inventory.getItemFromInteractedChest(field.x).ItemAmount - amountForHold));

					Player.Inventory.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					Player.Inventory.setHoldItem(ItemField(0, 0));
				}
			}
			else if(Player.Inventory.getItemFromInteractedChest(field.x).ItemId != 0)
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
			if (Player.Inventory.getItemFromInteractedChest(field.x).ItemId != 0)
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

void Engine::updateTile(const sf::Vector2u &tileIndex)
{
	unsigned objectId = GameWorld->getLocalMapTileObjectId(tileIndex);
	if (objectId == 0) { return; }
	float Time = GameClock.getElapsedTime().asSeconds();

	if (GameWorld->getLocalMapTileObject(tileIndex)->getType() == ObjectType::Sapling)
	{
		float Time = GameClock.getElapsedTime().asSeconds();
		float plantTime = dynamic_cast<SaplingObject*>(GameWorld->getLocalMapTileObject(tileIndex))->getPlantTime();
		float growTime = dynamic_cast<SaplingDef*>(Objects->getDefinition(objectId))->getGrowTime();

		if (sf::seconds(Time) >= sf::seconds(plantTime) + sf::seconds(growTime))
		{
			unsigned GrowToId = Objects->getDefIdbyName(dynamic_cast<SaplingDef*>(Objects->getDefinition(objectId))->getGrowTo());
			if (GrowToId != 0)
			{
				GameWorld->removeLocalMapTileObject(tileIndex);
				GWorldManager.placeObject(tileIndex, GrowToId);
			}
		}
	}
}

void Engine::manageConsole(sf::Event &event, const sf::Vector2f &mousePos, bool isMouseRClick)
{
	if (!GameConsole.getEnable()) { return; }

	auto tmp = GameConsole(event, mousePos, isMouseRClick);

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
				GameConsole.giveItemCheck(tmp, *Items, Player.Inventory);
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
	if (ObjectID > Objects->getSize()) { return; }

	shp.setPosition(sf::Vector2f(World::getNormalPosition(static_cast<sf::Vector2i>(objectIndex))));
	if(preObjectId != ObjectID)
	{
		shp.setTexture(mediaContainer.getTexture(TextureContainer::ObjectTextures, ObjectID), true);
	}
	preObjectId = ObjectID;
	window.draw(shp);

	/*
	int sizeY = Objects->getDefinition(ObjectID)->getSize().y;
	if (sizeY < 0)
	{
		shp.setPosition(sf::Vector2f(World::getNormalPosition(sf::Vector2i(objectIndex.x, objectIndex.y+(sizeY+1)))));
		shp.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE * (-sizeY)));
		shp.setTexture(mediaContainer.getTexture(TextureContainer::ObjectTextures, ObjectID), true);
		window.draw(shp);
		shp.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		return;
	}
	*/
}

Engine::Engine(GameVars &v1, unsigned MaxTileDrawRange)
	:Player(sf::RectangleShape{sf::Vector2f(48,64)}, sf::Vector2f(), 20.0f, 10.0f, 5.0f)
{
	ErrorHandler::clearLogFile();
	loadGameComponents();

	std::vector<RecipeDef> PlayerRecipesDef;
	GameComponentsLoader::LoadRecipeDefFromFile(PlayerRecipesDef, "Data/Recipes/PlayerRecipes.xml");
	Crafting.loadPlayerRecipes(makeFromDef::makeRecipe(PlayerRecipesDef, *Items));
	Crafting.usePlayerRecipes();
	Crafting.AssingItemDef(Items);

	LyingItems.init(v1.MaxNumberOfLyingItems, static_cast<sf::Vector2f>(sf::Vector2u(v1.PlayerPickUpItemsRange, v1.PlayerPickUpItemsRange)));
	TileDrawRange = MaxTileDrawRange;

	std::vector<StructureDef> StructuresDef;
	GameComponentsLoader::LoadStructuresDef(StructuresDef);
	std::vector<Structure> Structures = makeFromDef::makeStructure(StructuresDef, *Objects);

	std::vector<BiomeValuesDef> MapsDef;
	MapsDef.push_back(BiomeValuesDef());
	GameComponentsLoader::LoadLocalMapVariables(MapsDef);
	std::vector<BiomeValues> MapsVars = makeFromDef::makeBiome(MapsDef, *Objects, Structures);

	GWorldManager.setStructuresAmountInLocalMap(v1.StructuresPerLocalMap);
	GWorldManager.AssingClock(GameClock);
	GWorldManager.AssingItemsDef(Items);
	GWorldManager.AssingObjectsDef(Objects);
	GWorldManager.AssingStructures(Structures);
	GWorldManager.AssingWorld(GameWorld);
	GWorldManager.AssingLocalMapsBuilderVars(MapsVars);
	GWorldManager.buildLocalMap(TerrainType::Grass, v1.LocalMapSize);

	Player.Stats = Entities->getContainer().front().getStats();
	Player.pushTexture(mediaContainer.getTexture(TextureContainer::EntitiesTextures, 1));
	sf::Vector2f playerSpawnPos = GWorldManager.getSpawnPosition();

	ErrorHandler::log(std::string("Spawn Player position:"));
	ErrorHandler::log("Tile Y " + std::to_string(World::getTiledPosition(playerSpawnPos).y));
	ErrorHandler::log("Tile X " + std::to_string(World::getTiledPosition(playerSpawnPos).x));

	Player.setPosition(playerSpawnPos);
	Player.setSpawnPoint(playerSpawnPos);

	GMonsterManager.assingMonsterWorld(GameWorld);
	GMonsterManager.addEntityToObserved(&Player);
}

Engine::~Engine()
{
	ErrorHandler::log("Clear data");
}

void Engine::operator()(IslandApp &app, char key, mouseWheel last, bool isMouseClick)
{
	LyingItems.clearOldItems(GameClock.getElapsedTime());

	checkPlayerBehaviour();
	checkPlayerEnvironment();

	app.getIslandWindow()->setView(*app.getIslandView());

	auto Window = app.getIslandWindow();
	GameConsole.setPosition(Window->mapPixelToCoords(sf::Vector2i(800, 200)));

	if (key == '`')
	{
		GameConsole.setEnable(!GameConsole.getEnable());
		GameConsole.setTexboxEnable(!GameConsole.getTextboxEnable());
	}

	if (GameGui.Eq.isEnable)
	{
		sf::Vector2f mousePosInWorld = app.getMousePosInWorld();
		for (size_t i = 0; i < PlayerInventorySize; i++)
		{
			if (i < 3)
			{
				if (GameGui.Eq.ArmorEquipment[i].isHover && isMouseClick)
				{
					checkGuiOperations(EquipmentType::Armor, sf::Vector2u(i, 0));
				}
			}

			if (GameGui.Hud.Belt[i].isHover && isMouseClick)
			{
				checkGuiOperations(EquipmentType::Belt, sf::Vector2u(i, 0));
			}

			for (size_t j = 0; j < PlayerInventorySize; j++)
			{
				if (GameGui.Eq.Equipment[i][j].isHover && isMouseClick)
				{
					checkGuiOperations(EquipmentType::Inventory, sf::Vector2u(i, j));
				}
			}
		}
		if (Player.Inventory.isInteractedChestExist())
		{
			for (size_t i = 0; i < Player.Inventory.getInteractedChestSize(); i++)
			{
				if (GameGui.Chest.ChestContain[i].isHover && isMouseClick)
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

				if (j.isHover && isMouseClick)
				{
					Crafting.setSelectedRecipe(RecipeNumber);
				}

				RecipeNumber++;
			}
		}
		if (isMouseClick)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*app.getIslandWindow());
			if (GameGui.Craft.RecipeInfo.CraftButton.isClick(mousePos))
			{
				ItemField craftedItem = Crafting.craftItemFromRecipe(Player.Inventory);
				if (!craftedItem.isEmpty())
				{
					Player.Inventory.pushItem(craftedItem, Items->getDefinition(craftedItem.ItemId)->getMaxStack());
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
	}
	else if (!GameGui.Eq.isEnable && Player.Inventory.getHoldItem().isEmpty() && isMouseClick)
	{
		ItemField item = Player.Inventory.getHandInventoryField(GameGui.Hud.ActiveBeltField);
		sf::Vector2f mousePos = app.getMousePosInWorld();
		sf::Vector2u objectPos(static_cast<sf::Vector2u>(mousePos / TILE_SIZE));

		if (!item.isEmpty())
		{
			if (sf::FloatRect(sf::Vector2f(), sf::Vector2f(GameWorld->getLocalMapSize() * TILE_SIZE,
				GameWorld->getLocalMapSize() * TILE_SIZE)).contains(mousePos))
			{
				if (GWorldManager.placeObject(objectPos, item.ItemId))
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
						Yield objectYield = Objects->getDefinition(objectId)->getYield();
						if (objectYield.first != "NULL")
						{
							if (objectYield.first == "SELF")
							{
								LyingItems.pushNewItem(GameClock.getElapsedTime(), mousePos, ItemField(objectId, objectYield.second));
							}
							else
							{
								ItemField item;
								item.ItemId = Items->getDefIdbyName(objectYield.first);
								item.ItemAmount = objectYield.second;
								LyingItems.pushNewItem(GameClock.getElapsedTime(), mousePos, item);
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
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

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) || !GameGui.Eq.isEnable)
	{
		if (Player.Inventory.getHoldItem().ItemId != 0)
		{
			LyingItems.pushNewItem(GameClock.getElapsedTime(), app.getMousePosInWorld(), Player.Inventory.getHoldItem());
			Player.Inventory.setHoldItem(ItemField(0, 0));
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
	sf::RectangleShape TileShape;
	sf::RectangleShape ObjectShape;
	TileShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	ObjectShape = TileShape;

	int iTileDrawRange = static_cast<int>(TileDrawRange);
	int MapSize = static_cast<int>(GameWorld->getLocalMapSize());
	TerrainType preTile = TerrainType::Null;
	size_t preObjectId = 0;

	for (int i = PlayerPosToTile.x - iTileDrawRange; i < PlayerPosToTile.x + iTileDrawRange + 1; i++)
	{
		if (i < 0) { continue; }
		if (i > MapSize - 1) { break; }
		for (int j = PlayerPosToTile.y - iTileDrawRange; j < PlayerPosToTile.y + iTileDrawRange + 1; j++)
		{
			if (j < 0) { continue; }
			if (j > MapSize - 1) { break; }
			drawTile(preTile, static_cast<sf::Vector2u>(sf::Vector2i(i, j)), *app.getIslandWindow(), TileShape);
			drawObject(preObjectId, static_cast<sf::Vector2u>(sf::Vector2i(i, j)), *app.getIslandWindow(), ObjectShape);
			updateTile(static_cast<sf::Vector2u>(sf::Vector2i(i, j)));
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
				LyingItems.getItem(i).ItemId), true);
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

	sf::Text amountItem;
	amountItem.setCharacterSize(16);
	amountItem.setFont(GameGui.GuiFont);
	amountItem.setFillColor(sf::Color(255, 0, 0, 255));

	sf::RectangleShape FieldShape;
	FieldShape.setSize(sf::Vector2f(EquipmentFieldSize, EquipmentFieldSize));
	sf::RectangleShape TextureFieldShape;
	TextureFieldShape.setSize(FieldShape.getSize());
	sf::Vector2f mousePosition = app.getMousePosInWorld();

	if (!Player.Inventory.getHoldItem().isEmpty())
	{
		sf::RectangleShape holdItemRep;
		holdItemRep.setSize(sf::Vector2f(EquipmentFieldSize, EquipmentFieldSize));
		holdItemRep.setTexture(mediaContainer.getTexture(TextureContainer::ItemsTextures,
			Player.Inventory.getHoldItem().ItemId));
		holdItemRep.setPosition(app.getMousePosInWorld());

		app.draw(holdItemRep);
	}


	for (size_t i = 0; i < PlayerInventorySize; i++)
	{
		amountItem.setString("");

		GameGui.Hud.Belt[i].checkIsHover(mousePosition);
		sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords(GameGui.Hud.Belt[i].ScreenPosition);
		GameGui.Hud.Belt[i].Position = newPos;
		GameGui.Hud.Belt[i].pushChangesToRectangleShape(FieldShape);
		ItemField temp = Player.Inventory.getHandInventoryField(i);

		pushItemTextureToRect(newPos,temp.ItemId, TextureFieldShape);
		amountItem.setPosition(FieldShape.getPosition());

		if (temp.ItemId != 0) { amountItem.setString(std::to_string(temp.ItemAmount)); }
		if (i == GameGui.Hud.ActiveBeltField) { FieldShape.setFillColor(ActiveBeltFieldColor); }

		app.draw(FieldShape);
		if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
		if (amountItem.getString().getSize() > 0) { app.draw(amountItem); }
	}

	if (GameGui.Eq.isEnable && !GameGui.Craft.isEnable)
	{
		sf::Vector2u field;
		for (field.x = 0; field.x < PlayerInventorySize; field.x++)
		{
			for (field.y = 0; field.y < PlayerInventorySize; field.y++)
			{
				if (field.x < 3)
				{
					sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords
					(GameGui.Eq.ArmorEquipment[field.x].ScreenPosition);

					GameGui.Eq.ArmorEquipment[field.x].checkIsHover(mousePosition);
					GameGui.Eq.ArmorEquipment[field.x].Position = newPos;

					GameGui.Eq.ArmorEquipment[field.x].pushChangesToRectangleShape(FieldShape);
					ItemField temp = Player.Inventory.getArmorInventoryField(field.x);
					pushItemTextureToRect(newPos, temp.ItemId, TextureFieldShape);

					app.draw(FieldShape);
					if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
				}

				amountItem.setString("");

				GameGui.Eq.Equipment[field.x][field.y].checkIsHover(mousePosition);
				sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords(GameGui.Eq.Equipment[field.x][field.y].ScreenPosition);
				GameGui.Eq.Equipment[field.x][field.y].Position = newPos;

				GameGui.Eq.Equipment[field.x][field.y].pushChangesToRectangleShape(FieldShape);
				ItemField temp = Player.Inventory.getInventoryField(field);
				pushItemTextureToRect(newPos, temp.ItemId, TextureFieldShape);
				amountItem.setPosition(newPos);

				if (temp.ItemAmount != 0) { amountItem.setString(std::to_string(temp.ItemAmount)); }

				app.draw(FieldShape);
				if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
				if (amountItem.getString().getSize() > 0) { app.draw(amountItem); }
			}
		}
	}

	if (Player.Inventory.isInteractedChestExist() && GameGui.Eq.isEnable)
	{
		for (size_t i = 0; i < Player.Inventory.getInteractedChestSize(); i++)
		{
			GameGui.Chest.ChestContain[i].checkIsHover(mousePosition);
			sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords(GameGui.Chest.ChestContain[i].ScreenPosition);
			GameGui.Chest.ChestContain[i].Position = newPos;
			GameGui.Chest.ChestContain[i].pushChangesToRectangleShape(FieldShape);
			ItemField temp = Player.Inventory.getItemFromInteractedChest(i);
			pushItemTextureToRect(newPos, temp.ItemId, TextureFieldShape);
			amountItem.setPosition(newPos);

			if (temp.ItemAmount != 0) { amountItem.setString(std::to_string(temp.ItemAmount)); }
			
			app.draw(FieldShape);
			if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
			if (amountItem.getString().getSize() > 0) { app.draw(amountItem); }
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
					pushItemTextureToRect(newPos, temp.ItemId, TextureFieldShape);
					if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
					if (!temp.isEmpty())
					{
						amountItem.setPosition(newPos);
						amountItem.setString(std::to_string(temp.ItemAmount));
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
	if (itemId > 0) { rect.setTexture(mediaContainer.getTexture(TextureContainer::ItemsTextures, itemId)); }
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