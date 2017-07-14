#include "Engine.hpp"

void Engine::loadGameComponents()
{
	GameComponentsLoader loader;
	std::string objectGraphicsfile;
	std::vector<sf::IntRect> objectTextureCords;

	loader.LoadObjectDefFromFile(Objects.getContainer(), objectGraphicsfile, objectTextureCords);
	for (size_t i = 0; i < Objects.getSize(); i++)
	{
		ErrorHandler::log("Load Object: " + Objects.getDefinition(i)->getName() + " Type: " + std::to_string(static_cast<int>(Objects.getDefinition(i)->getType())));
	}

	for (auto & i : objectTextureCords)
	{
		mediaContainer.pushTexture(TextureContainer::ObjectTextures, objectGraphicsfile, i);
		mediaContainer.pushTexture(TextureContainer::ItemsTextures, objectGraphicsfile, i);
	}

	loader.GenerateItemsFromObjectDef(Objects.getContainer(), Items.getContainer());

	std::string itemGraphicsFile;
	std::vector<sf::IntRect> itemTextureCords;
	loader.LoadItemDefFromFile(Items.getContainer(), itemGraphicsFile, itemTextureCords);

	for (auto & i : itemTextureCords)
	{
		mediaContainer.pushTexture(TextureContainer::ItemsTextures, itemGraphicsFile, i);
	}

	for (size_t i = 0; i < Items.getSize(); i++)
	{
		ErrorHandler::log("Load Item: " + Items.getDefinition(i)->getName() + " MaxStack: " + std::to_string(Items.getDefinition(i)->getMaxStack())
			+ " Type: " + std::to_string(static_cast<int>(Items.getDefinition(i)->getType())));
	}
}

void Engine::checkPlayerEnvironment()
{
	sf::Vector2f playerCollectRectSize(128, 128);
	sf::Vector2f playerCollectRectPos = player.getPosition() - sf::Vector2f(playerCollectRectSize.x / 2,
		playerCollectRectSize.y / 2);

	for (size_t i = 0; i < LyingItems.getSize(); i++)
	{
		if (CollisionDetect::isPointInRectangle(LyingItems.getPosition(i),
			playerCollectRectPos, playerCollectRectSize))
		{
			ItemField temp = LyingItems.getItem(i);
			player.pushItemToPlayer(temp, Items);
			if (temp.isClear())
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

void Engine::checkPlayerBehaviour(IslandApp &app)
{
	sf::Vector2f movevctr;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		//GameGui.popInteractionWithChest();
		movevctr.x -= 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		//GameGui.popInteractionWithChest();
		movevctr.x += 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//GameGui.popInteractionWithChest();
		movevctr.y -= 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		//GameGui.popInteractionWithChest();
		movevctr.y += 5;
	}

	player.move(movevctr);
}

void Engine::spawnPlayer()
{
	const int MaxPosition = WorldSize - 10;

	srand(static_cast<unsigned int>(time(NULL)));
	sf::Vector2f spawnPoint;

	while (true)
	{
		spawnPoint = sf::Vector2f(static_cast<float>(rand() % MaxPosition * 64),
			static_cast<float>(rand() % MaxPosition * 64));

		if (GameWorld.isPlaceImpassable(sf::Vector2f(spawnPoint.y,spawnPoint.x)) == false)
		{
			ErrorHandler::log(std::string("Spawn player position:"));
			ErrorHandler::log("Tile Y " + std::to_string(Map::getTiledPosition(spawnPoint).y));
			ErrorHandler::log("Tile X " + std::to_string(Map::getTiledPosition(spawnPoint).x));
			break;
		}
	}
	player.setPosition(spawnPoint);
	player.setSpawnPoint(spawnPoint);
}

void Engine::checkGuiOperations(EquipmentType type, sf::Vector2u field)
{
	unsigned holdedItemId = player.getHoldItem().ItemId;
	switch (type)
	{
	case EquipmentType::Inventory:
		if (holdedItemId != 0)
		{
			if (player.getInventoryField(field).ItemId == holdedItemId)
			{
				player.setInventoryField(field, ItemField(holdedItemId,
					player.getInventoryField(field).ItemAmount + player.getHoldItem().ItemAmount));

				unsigned maxStack = Items.getDefinition(holdedItemId)->getMaxStack();
				if (player.getInventoryField(field).ItemAmount >= maxStack)
				{
					unsigned amountForHold = player.getInventoryField(field).ItemAmount - maxStack;
					player.setInventoryField(field, ItemField(holdedItemId,
						player.getInventoryField(field).ItemAmount - amountForHold));

					player.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					player.setHoldItem(ItemField(0,0));
				}

			}
			else if(player.getInventoryField(field).ItemId != 0)
			{
				ItemField temp(player.getInventoryField(field));
				player.setInventoryField(field, player.getHoldItem());
				player.setHoldItem(temp);
			}
			else
			{
				player.setInventoryField(field, player.getHoldItem());
				player.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (player.getInventoryField(field).ItemId != 0)
			{
				player.setHoldItem(player.getInventoryField(field));
				player.setInventoryField(field, ItemField());
			}
		}
		break;
	case EquipmentType::Armor:
		if (holdedItemId != 0)
		{
			if (player.getArmorInventoryField(field.x).ItemId != 0)
			{
				ItemField temp(player.getArmorInventoryField(field.x));
				player.setArmorField(field.x, player.getHoldItem());
				player.setHoldItem(temp);
			}
			else
			{
				player.setArmorField(field.x, player.getHoldItem());
				player.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (player.getArmorInventoryField(field.x).ItemId != 0)
			{
				player.setHoldItem(player.getArmorInventoryField(field.x));
				player.setArmorField(field.x, ItemField());
			}
		}
		break;
	case EquipmentType::Belt:
		if (holdedItemId != 0)
		{
			if (player.getHandInventoryField(field.x).ItemId == holdedItemId)
			{
				player.setHandInventoryField(field.x, ItemField(holdedItemId,
					player.getHandInventoryField(field.x).ItemAmount + player.getHoldItem().ItemAmount));

				unsigned maxStack = Items.getDefinition(holdedItemId)->getMaxStack();
				if (player.getHandInventoryField(field.x).ItemAmount >= maxStack)
				{
					unsigned amountForHold = player.getHandInventoryField(field.x).ItemAmount - maxStack;
					player.setHandInventoryField(field.x, ItemField(holdedItemId,
						player.getHandInventoryField(field.x).ItemAmount - amountForHold));

					player.setHoldItem(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					player.setHoldItem(ItemField(0, 0));
				}

			}
			else if (player.getHandInventoryField(field.x).ItemId != 0)
			{
				ItemField temp(player.getHandInventoryField(field.x));
				player.setHandInventoryField(field.x, player.getHoldItem());
				player.setHoldItem(temp);
			}
			else
			{
				player.setHandInventoryField(field.x, player.getHoldItem());
				player.setHoldItem(ItemField(0, 0));
			}
		}
		else
		{
			if (player.getHandInventoryField(field.x).ItemId != 0)
			{
				player.setHoldItem(player.getHandInventoryField(field.x));
				player.setHandInventoryField(field.x, ItemField());
			}
		}
		break;
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
		for (size_t i = GameConsole.getCommandsSize() - 1; i > 0; i--)
		{
			if (begPos.y < GameConsole.getWindow().getPosition().y) { break; }
			consoleText.setString(GameConsole.getText(i));
			consoleText.setPosition(begPos);
			app.draw(consoleText);
			begPos -= sf::Vector2f(0, 16);
		}
	}
}

void Engine::manageConsole(sf::Event &event, sf::Vector2f mousePos, bool isMouseRClick)
{
	if (!GameConsole.getEnable()) { return; }

	auto tmp = GameConsole(event, mousePos, isMouseRClick);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
			if (GameConsole.getLastHistoryCmdNumber() == 0)
			{
				GameConsole.setCurrentText(GameConsole.getLastCmdFromHistory());
				GameConsole.getLastHistoryCmdNumber() = GameConsole.getHistorySize() - 2;
			}
			else
			{
				GameConsole.setCurrentText(GameConsole.getCmdFromHistory(GameConsole.getLastHistoryCmdNumber()));
				GameConsole.getLastHistoryCmdNumber() -= 1;
			}
	}

	if (tmp.size() > 0 )
	{
		GameConsole.getLastHistoryCmdNumber() = 0;
		if (tmp[0] == '/')
		{
			if (tmp == "/help")
			{
				GameConsole.pushText(std::string("Commands:/giveItem,/spawnmonster"));
				GameConsole.pushText(std::string("/placeObject,/settile,/settime"));
				GameConsole.pushText(std::string("/playerposition,/time,/worldsize"));
				GameConsole.pushText(std::string("/clear,/"));

				GameConsole.pushCommandToHistory(tmp);
			}
			else if(tmp == "/playerposition")
			{
				GameConsole.pushText(std::string("Pos-x: ") + std::to_string(player.getCharacterCenterPosition().x));
				GameConsole.pushText(std::string("Pos-y: ") + std::to_string(player.getCharacterCenterPosition().y));
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
			}
			else if(tmp == "/worldsize")
			{
				GameConsole.pushText(std::string("Size: ") + std::to_string(WorldSize));
				GameConsole.pushCommandToHistory(tmp);
			}
			else if(tmp.find("/giveItem") != std::string::npos)
			{
				GameConsole.giveItemCheck(tmp, Items, player);
			}
			else if(tmp.find("/placeObject") != std::string::npos)
			{
				GameConsole.placeObjectCheck(tmp, Objects, GameWorld);
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

void Engine::drawTile(sf::Vector2u tileIndex, sf::RenderWindow & window,sf::RectangleShape &shp)
{
	TILE tile = GameWorld.getTile(sf::Vector2u(tileIndex.y,tileIndex.x));
	if (tile == TILE::EMPTY) { return; }
	shp.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(tileIndex.x,tileIndex.y))));
	switch (tile)
	{
	case TILE::EMPTY:
		break;
	case TILE::DIRT:
		shp.setTexture(mediaContainer.getTexture(1,TextureContainer::TileTextures));
		break;
	case TILE::GRASS:
		shp.setTexture(mediaContainer.getTexture(2, TextureContainer::TileTextures));
		break;
	case TILE::ROCK:
		shp.setTexture(mediaContainer.getTexture(4, TextureContainer::TileTextures));
		break;
	case TILE::BRIGDE:
		shp.setTexture(mediaContainer.getTexture(2, TextureContainer::TileTextures));
		break;
	case TILE::CLOUD:
		shp.setTexture(mediaContainer.getTexture(6, TextureContainer::TileTextures));
		break;
	default:
		break;
	}
	window.draw(shp);
}

void Engine::drawObject(sf::Vector2u objectIndex, sf::RenderWindow & window, sf::RectangleShape &shp)
{
	unsigned ObjectID = GameWorld.getObjectId(sf::Vector2u(objectIndex.y,objectIndex.x));
	if (ObjectID == 0) { return; }
	if (ObjectID > Objects.getSize()) { return; }

	int sizeY = Objects.getDefinition(ObjectID)->getSize().y;
	if (sizeY < 0)
	{
		shp.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(objectIndex.x, objectIndex.y+(sizeY+1)))));
		shp.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE * (-sizeY)));
		shp.setTexture(mediaContainer.getTexture(ObjectID, TextureContainer::ObjectTextures), true);
		window.draw(shp);
		shp.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		return;
	}

		shp.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(objectIndex.x, objectIndex.y))));
		shp.setTexture(mediaContainer.getTexture(ObjectID, TextureContainer::ObjectTextures), true);
		window.draw(shp);
}

bool Engine::checkPlayerPos()
{
	sf::Vector2f playerPos = player.getCharacterCenterPosition();
	if (GameWorld.isPlaceImpassable(sf::Vector2f(playerPos.y,playerPos.x)) == true) { return false; }

	return true;
}

Engine::~Engine()
{
	ErrorHandler::log("Clear data");
}

void Engine::init()
{
	mediaContainer.load(); ErrorHandler::log("Load media");
	loadGameComponents(); ErrorHandler::log("Load game components");

	player.set(mediaContainer.getTexture(1,TextureContainer::CharacterTextures), sf::Vector2f(100, 100),sf::Vector2f(10,8));
	camera.setSize(sf::Vector2f(1280, 1024));

	GameWorld.init();
	ErrorHandler::log("Generate map");
	ErrorHandler::log("Map Size " + std::to_string(WorldSize) + " x " + std::to_string(WorldSize));
	spawnPlayer();
}

void Engine::operator()(IslandApp &app,char key,mouseWheel last, bool isMouseClick)
{
	LyingItems.clearOldItems(GameClock.getElapsedTime());

	checkPlayerBehaviour(app);
	checkPlayerEnvironment();

	app.getIslandWindow()->setView(camera);

	auto Window = app.getIslandWindow();
	GameConsole.setPosition(Window->mapPixelToCoords(sf::Vector2i(800, 200)));

	if (key == '`') { GameConsole.setEnable(!GameConsole.getEnable()); }

	if (!checkPlayerPos())
	{ 
		ErrorHandler::log("Player move above map");
		ErrorHandler::log("Pos:X" + std::to_string(player.getCharacterCenterPosition().x) +
			" :Y " + std::to_string(+player.getCharacterCenterPosition().y));
	}

	if (GameGui.Eq.isEnable)
	{
		sf::Vector2f mousePosInWorld = app.getMousePosInWorld();
		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			if (i < 3)
			{
				if (GameGui.Eq.ArmorEquipment[i].isHover)
				{
					if (isMouseClick)
					{
						checkGuiOperations(EquipmentType::Armor, sf::Vector2u(i, 0));
					}
				}
			}

			if (GameGui.Hud.Belt[i].isHover)
			{
				if (isMouseClick)
				{
					checkGuiOperations(EquipmentType::Belt, sf::Vector2u(i, 0));
				}
			}

			for (size_t j = 0; j < PlayerFieldsNumber; j++)
			{
				if (GameGui.Eq.Equipment[i][j].isHover)
				{
					if (isMouseClick)
					{
						checkGuiOperations(EquipmentType::Inventory, sf::Vector2u(i, j));
					}
				}
			}
		}
	}
	else if (!GameGui.Eq.isEnable && player.getHoldItem().isClear() && isMouseClick)
		{
			unsigned itemId = player.getHandInventoryField(GameGui.Hud.ActiveBeltField).ItemId;
			sf::Vector2f mousePos = app.getMousePosInWorld();
			sf::Vector2u objectPos(static_cast<unsigned>(mousePos.y / TILE_SIZE),
				static_cast<unsigned>(mousePos.x / TILE_SIZE));

			if (itemId != 0)
			{
				if (Items.getDefinition(itemId)->getType() == ItemType::Placeable)
				{
					if (CollisionDetect::isPointInRectangle(mousePos,sf::Vector2f(0,0),
						sf::Vector2f(WorldSize * TILE_SIZE,WorldSize * TILE_SIZE)))
					{
						if (player.getHandInventoryField(GameGui.Hud.ActiveBeltField).ItemAmount == 0)
						{
							player.setHandInventoryField(GameGui.Hud.ActiveBeltField, ItemField(0, 0));
						}
						
						if (GameWorld.placeObject(objectPos, itemId, Objects.getContainer(),GameClock.getElapsedTime()))
						{
							ItemField temp = player.getHandInventoryField(GameGui.Hud.ActiveBeltField);
							temp -= 1;
							player.setHandInventoryField(GameGui.Hud.ActiveBeltField, temp);
						}
					}
				}
			}
			else
			{
				unsigned objectId = GameWorld.getObjectId(objectPos);
				if (objectId > 0)
				{
					sf::Time timeAtMouseClick = GameClock.getElapsedTime();
					sf::Time timeOfMouseClickHold;

					while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						timeOfMouseClickHold = GameClock.getElapsedTime();
						if (timeOfMouseClickHold.asMilliseconds() - timeAtMouseClick.asMilliseconds() >= 101)
						{
							Yield objectYield = Objects.getDefinition(objectId)->getYield();
							if (objectYield.first != "NULL")
							{
								if (objectYield.first == "SELF")
								{
									LyingItems.pushNewItem(GameClock.getElapsedTime(), mousePos, ItemField(objectId, objectYield.second));
								}
								else
								{
									ItemField item;
									item.ItemId = Items.getDefIdbyName(objectYield.first);
									item.ItemAmount = objectYield.second;
									LyingItems.pushNewItem(GameClock.getElapsedTime(), mousePos, item);
								}
							}
							GameWorld.clearObject(objectPos);
							break;
						}
					}
				}
			}
		}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2f mousePos = app.getMousePosInWorld();
		sf::Vector2u objectPos(static_cast<unsigned>(mousePos.y / TILE_SIZE), 
			static_cast<unsigned>(mousePos.x / TILE_SIZE));

			if (GameWorld.getObjectType(objectPos) == ObjectType::Chest)
			{
				//GameGui.pushInteractionWithChest(dynamic_cast<ChestObject*>(GameWorld.getObject(objectPos)));
			}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) || !GameGui.Eq.isEnable)
	{
		if (player.getHoldItem().ItemId != 0)
		{
			LyingItems.pushNewItem(GameClock.getElapsedTime(), app.getMousePosInWorld(), player.getHoldItem());
			player.setHoldItem(ItemField(0, 0));
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
	GameGui.Hud.pushNewValuesForHpInfo(200, static_cast<unsigned>(player.getHP()));
	GameGui.Hud.pushNewValuesForMpInfo(200, static_cast<unsigned>(player.getMP()));
}

void Engine::drawWorld(IslandApp & app)
{
	sf::Vector2i PlayerPosToTile = Map::getTiledPosition(player.getCharacterCenterPosition());
	sf::RectangleShape TileShape,
		LyingItemShape;
	TileShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	LyingItemShape.setSize(sf::Vector2f(32, 32));

	for (int i = PlayerPosToTile.x - 30; i < PlayerPosToTile.x + 31; i++)
	{
		for (int j = PlayerPosToTile.y - 30; j < PlayerPosToTile.y + 31; j++)
		{
			if (j < 0 || i < 0) { continue; }
			if (j >= WorldSize || i >= WorldSize) { break; }
			drawTile(static_cast<sf::Vector2u>(sf::Vector2i(i, j)), *app.getIslandWindow(), TileShape);
			drawObject(static_cast<sf::Vector2u>(sf::Vector2i(i, j)), *app.getIslandWindow(), TileShape);
		}
	}
	sf::Vector2f cameraPos = camera.getCenter() - sf::Vector2f(camera.getSize().x / 2, camera.getSize().y / 2);
	for (size_t i = 0; i < LyingItems.getSize(); i++)
	{
		if (CollisionDetect::isPointInRectangle(LyingItems.getPosition(i), cameraPos, camera.getSize()))
		{
			LyingItemShape.setPosition(LyingItems.getPosition(i));
			LyingItemShape.setTexture(mediaContainer.getTexture(LyingItems.getItem(i).ItemId,
				TextureContainer::ItemsTextures), true);
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

	if (player.getHoldItem().ItemId != 0)
	{
		sf::RectangleShape holdItemRep;
		holdItemRep.setSize(sf::Vector2f(EquipmentFieldSize, EquipmentFieldSize));
		holdItemRep.setTexture(mediaContainer.getTexture(player.getHoldItem().ItemId,
			TextureContainer::ItemsTextures));
		holdItemRep.setPosition(app.getMousePosInWorld());

		app.draw(holdItemRep);
	}


	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		amountItem.setString("");

		GameGui.Hud.Belt[i].checkIsHover(mousePosition);
		sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords(getScreenPositionForBeltEquipmentField(i));
		GameGui.Hud.Belt[i].Position = newPos;
		GameGui.Hud.Belt[i].pushChangesToRectangleShape(FieldShape);
		ItemField temp = player.getHandInventoryField(i);
		pushChangesToEquipmentField(newPos,temp.ItemId, TextureFieldShape);
		amountItem.setPosition(FieldShape.getPosition());

		if (temp.ItemAmount != 0) { amountItem.setString(std::to_string(temp.ItemAmount)); }

		app.draw(FieldShape);
		if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
		if (amountItem.getString().getSize() > 0) { app.draw(amountItem); }
	}

	if (!GameGui.Eq.isEnable) { return; }

	sf::Vector2u field = sf::Vector2u(0,0);
	for (field.x; field.x < PlayerFieldsNumber; field.x++)
	{
		if (field.x < 3)
		{
			sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords
			(getScreenPositionForArmorEquipmentField(field.x));

			GameGui.Eq.ArmorEquipment[field.x].checkIsHover(mousePosition);
			GameGui.Eq.ArmorEquipment[field.x].Position = newPos;

			GameGui.Eq.ArmorEquipment[field.x].pushChangesToRectangleShape(FieldShape);
			ItemField temp = player.getArmorInventoryField(field.x);
			pushChangesToEquipmentField(newPos, temp.ItemId, TextureFieldShape);

			app.draw(FieldShape);
			if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
		}

		for (field.y; field.y < PlayerFieldsNumber; field.y++)
		{
			amountItem.setString("");

			GameGui.Eq.Equipment[field.x][field.y].checkIsHover(mousePosition);
			sf::Vector2f newPos = app.getIslandWindow()->mapPixelToCoords(getScreenPositionForEquipmentField(field));
			GameGui.Eq.Equipment[field.x][field.y].Position = newPos;

			GameGui.Eq.Equipment[field.x][field.y].pushChangesToRectangleShape(FieldShape);
			ItemField temp = player.getInventoryField(field);
			pushChangesToEquipmentField(newPos, temp.ItemId, TextureFieldShape);
			amountItem.setPosition(newPos);

			if (temp.ItemAmount != 0) { amountItem.setString(std::to_string(temp.ItemAmount)); }

			app.draw(FieldShape);
			if (TextureFieldShape.getTexture() != nullptr) { app.draw(TextureFieldShape); }
			if (amountItem.getString().getSize() > 0) { app.draw(amountItem); }
		}
	}

	/*
	if (GameGui.isChestExist())
	{
		EquipmentField chestField;
		chestField.create(sf::Vector2f(), DefaultEqFieldSize, DefaultEqFieldColor);
		sf::Vector2i screenCenter = static_cast<sf::Vector2i>(app.getIslandWindow()->getSize());
		screenCenter.x /= 2;
		screenCenter.y /= 8;
		sf::Vector2f pos = app.getIslandWindow()->mapPixelToCoords(screenCenter);
		pos.y += -10.0f;

		for (size_t i = 0; i < GameGui.getChestCapSize(); i++)
		{
			if (i != 0 && i % 5 == 0) 
			{ 
				pos.y += DefaultEqFieldSize + 20;
				pos.x += -((5 * 20.0f) + 5 * 64.0f);
			}

			ItemField temp = GameGui.getChestItem(i);
			if (temp.ItemId != 0 && temp.ItemAmount != 0)
			{
				chestField.setTexture(mediaContainer.getTexture(temp.ItemId, TextureContainer::ItemsTextures));
				amountItem.setString(std::to_string(temp.ItemAmount));

				app.draw(chestField.FieldRect);
				app.draw(chestField.TextureRect);
				app.draw(amountItem);
			}
			else
			{
				app.draw(chestField.FieldRect);
			}
			chestField.setPosition(pos);
			amountItem.setPosition(pos);

			pos.x += DefaultEqFieldSize + 20;
		}

	}
	*/
		
}

void Engine::pushChangesToEquipmentField(sf::Vector2f pos, unsigned itemId, sf::RectangleShape & rect)
{
	rect.setPosition(pos);
	if (itemId > 0) { rect.setTexture(mediaContainer.getTexture(itemId, TextureContainer::ItemsTextures)); }
	else { rect.setTexture(nullptr); }
}

void Engine::DrawAll(IslandApp &app)
{
	camera.setCenter(player.getCharacterCenterPosition());

	drawWorld(app);
	drawPlayerGui(app);
	drawConsole(app);

	app.draw(*player.getShape());
}