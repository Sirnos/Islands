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
		movevctr.x -= 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movevctr.x += 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movevctr.y -= 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
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

void Engine::pushChangesToGui()
{
	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		if (i < 3)
		{
			GameGui.EquipmentGui.clearArmorField(i);
			if (GameGui.EquipmentGui.getHoverFromArmorField(i))
			{
				GameGui.EquipmentGui.getArmorFieldRect(i).setFillColor(EqFieldColorWhenIsSelected);
			}

			unsigned ArmorItemId = player.getArmorInventoryField(i).ItemId;
			if (ArmorItemId != 0)
			{
				GameGui.EquipmentGui.pushTextureToArmorFields(i, mediaContainer.getTexture(ArmorItemId,
					TextureContainer::ItemsTextures));
			}
		}

		GameGui.HudGui.clearBeltField(i);
		if (i == GameGui.getNumberOfSelectedBeltField())
		{
			GameGui.HudGui.getBeltFieldRect(i).setFillColor(EqFieldColorWhenIsSelected);
		}
		if (GameGui.HudGui.getHoverFromBeltField(i))
		{
			GameGui.HudGui.getBeltFieldRect(i).setFillColor(EqFieldColorWhenIsSelected);
		}

		unsigned itemId = player.getHandInventoryField(i).ItemId;
		unsigned itemAmount = player.getHandInventoryField(i).ItemAmount;

		if (itemId != 0 && itemAmount != 0)
		{
			GameGui.HudGui.pushTextureToBeltField(i, mediaContainer.getTexture(itemId,
				TextureContainer::ItemsTextures));
		}
		else if(itemAmount == 0)
		{
			player.setHandInventoryField(i, ItemField());
		}

		for (size_t j = 0; j < PlayerFieldsNumber; j++)
		{
			GameGui.EquipmentGui.clearInventoryField(sf::Vector2u(i, j));
			if (GameGui.EquipmentGui.getHoverFromInventoryField(sf::Vector2u(i,j)))
			{
				GameGui.EquipmentGui.getFieldRect(sf::Vector2u(i, j)).setFillColor(EqFieldColorWhenIsSelected);
			}

			unsigned tempItemId = player.getInventoryField(sf::Vector2u(i, j)).ItemId;
			unsigned tempItemAmount = player.getInventoryField(sf::Vector2u(i, j)).ItemAmount;
			if (tempItemId != 0 && tempItemAmount != 0)
			{
				GameGui.EquipmentGui.pushTextureToFields(sf::Vector2u(i, j), mediaContainer.getTexture(tempItemId,
					TextureContainer::ItemsTextures));
			}
			else if(tempItemAmount == 0)
			{
				player.setInventoryField(sf::Vector2u(i, j), ItemField());
			}
		}

	}
}

void Engine::checkGuiOperations(EquipmentType type, sf::Vector2u field)
{
	unsigned holdedItemId = GameGui.getHoldedItem().ItemId;
	switch (type)
	{
	case EquipmentType::Inventory:
		if (holdedItemId != 0)
		{
			if (player.getInventoryField(field).ItemId == holdedItemId)
			{
				player.setInventoryField(field, ItemField(holdedItemId,
					player.getInventoryField(field).ItemAmount + GameGui.getHoldedItem().ItemAmount));

				unsigned maxStack = Items.getDefinition(holdedItemId)->getMaxStack();
				if (player.getInventoryField(field).ItemAmount >= maxStack)
				{
					unsigned amountForHold = player.getInventoryField(field).ItemAmount - maxStack;
					player.setInventoryField(field, ItemField(holdedItemId,
						player.getInventoryField(field).ItemAmount - amountForHold));

					GameGui.assingNewItemToHold(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					GameGui.clearHoldedItem();
				}

			}
			else if(player.getInventoryField(field).ItemId != 0)
			{
				ItemField temp(player.getInventoryField(field));
				player.setInventoryField(field, GameGui.getHoldedItem());
				GameGui.assingNewItemToHold(temp);
			}
			else
			{
				player.setInventoryField(field, GameGui.getHoldedItem());
				GameGui.clearHoldedItem();
			}
		}
		else
		{
			if (player.getInventoryField(field).ItemId != 0)
			{
				GameGui.assingNewItemToHold(player.getInventoryField(field));
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
				player.setArmorField(field.x, GameGui.getHoldedItem());
				GameGui.assingNewItemToHold(temp);
			}
			else
			{
				player.setArmorField(field.x, GameGui.getHoldedItem());
				GameGui.clearHoldedItem();
			}
		}
		else
		{
			if (player.getArmorInventoryField(field.x).ItemId != 0)
			{
				GameGui.assingNewItemToHold(player.getArmorInventoryField(field.x));
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
					player.getHandInventoryField(field.x).ItemAmount + GameGui.getHoldedItem().ItemAmount));

				unsigned maxStack = Items.getDefinition(holdedItemId)->getMaxStack();
				if (player.getHandInventoryField(field.x).ItemAmount >= maxStack)
				{
					unsigned amountForHold = player.getHandInventoryField(field.x).ItemAmount - maxStack;
					player.setHandInventoryField(field.x, ItemField(holdedItemId,
						player.getHandInventoryField(field.x).ItemAmount - amountForHold));

					GameGui.assingNewItemToHold(ItemField(holdedItemId, amountForHold));
				}
				else
				{
					GameGui.clearHoldedItem();
				}

			}
			else if (player.getHandInventoryField(field.x).ItemId != 0)
			{
				ItemField temp(player.getHandInventoryField(field.x));
				player.setHandInventoryField(field.x, GameGui.getHoldedItem());
				GameGui.assingNewItemToHold(temp);
			}
			else
			{
				player.setHandInventoryField(field.x, GameGui.getHoldedItem());
				GameGui.clearHoldedItem();
			}
		}
		else
		{
			if (player.getHandInventoryField(field.x).ItemId != 0)
			{
				GameGui.assingNewItemToHold(player.getHandInventoryField(field.x));
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
	shp.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(objectIndex.x, objectIndex.y))));
	shp.setTexture(mediaContainer.getTexture(ObjectID, TextureContainer::ObjectTextures),true);
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

	GameGui.create();
}

void Engine::operator()(IslandApp &app,char key,mouseWheel last, bool isMouseClick)
{
	LyingItems.clearOldItems(GameClock.getElapsedTime());

	checkPlayerBehaviour(app);
	checkPlayerEnvironment();

	camera.setCenter(player.getCharacterCenterPosition());
	app.getIslandWindow()->setView(camera);

	auto Window = app.getIslandWindow();

	if (key == '`') { GameConsole.setEnable(!GameConsole.getEnable()); }

	GameGui.setNewPosition(Window->mapPixelToCoords(GameGui.EquipmentGui.defaultEquipmentGuiPosOnScreen));
	GameGui.HudGui.setNewPosition(Window->mapPixelToCoords(GameGui.HudGui.HpInfoScreenPos),
		Window->mapPixelToCoords(GameGui.HudGui.MpInfoScreenPos),Window->mapPixelToCoords(GameGui.HudGui.BeltFieldPosOnScreen));
	GameConsole.setPosition(Window->mapPixelToCoords(sf::Vector2i(800, 200)));

	if (!checkPlayerPos())
	{ 
		ErrorHandler::log("Player move above map");
		ErrorHandler::log("Pos:X" + std::to_string(player.getCharacterCenterPosition().x) +
			" :Y " + std::to_string(+player.getCharacterCenterPosition().y));
	}

	if (GameGui.getIsEqGuiEnable())
	{
		sf::Vector2f mousePosInWorld = app.getMousePosInWorld();
		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			if (i < 3)
			{
				if (GameGui.EquipmentGui.getHoverFromArmorField(i))
				{
					GameGui.EquipmentGui.setHoverForArmorField(i, false);
				}
				sf::Vector2f fieldPos = GameGui.EquipmentGui.getArmorFieldRect(i).getPosition();

				if (GameGui.EquipmentGui.getArmorFieldRect(i).getGlobalBounds().contains(mousePosInWorld))
				{
					GameGui.EquipmentGui.setHoverForArmorField(i, true);
					if (isMouseClick)
					{
						checkGuiOperations(EquipmentType::Armor, sf::Vector2u(i, 0));
					}
				}
			}

			if (GameGui.HudGui.getHoverFromBeltField(i))
			{
				GameGui.HudGui.setHoverForBeltField(i, false);
			}
			sf::Vector2f beltFieldPos = GameGui.HudGui.getBeltFieldRect(i).getPosition();
			if (GameGui.HudGui.getBeltFieldRect(i).getGlobalBounds().contains(mousePosInWorld))
			{
				GameGui.HudGui.setHoverForBeltField(i, true);
				if (isMouseClick)
				{
					checkGuiOperations(EquipmentType::Belt, sf::Vector2u(i, 0));
				}
			}

			for (size_t j = 0; j < PlayerFieldsNumber; j++)
			{
				if (GameGui.EquipmentGui.getHoverFromInventoryField(sf::Vector2u(i, j)))
				{
					GameGui.EquipmentGui.setHoverForInventoryField(sf::Vector2u(i, j), false);
				}

				sf::Vector2f fieldPos = GameGui.EquipmentGui.getFieldRect(sf::Vector2u(i, j)).getPosition();
				if (GameGui.EquipmentGui.getFieldRect(sf::Vector2u(i,j)).getGlobalBounds().contains(mousePosInWorld))
				{
					GameGui.EquipmentGui.setHoverForInventoryField(sf::Vector2u(i, j), true);
					if (isMouseClick)
					{
						checkGuiOperations(EquipmentType::Inventory, sf::Vector2u(i, j));
					}
				}
			}
		}
	}
	else if (!GameGui.getIsEqGuiEnable() && GameGui.getHoldedItem().isClear() && isMouseClick)
		{
			unsigned itemId = player.getHandInventoryField(GameGui.getNumberOfSelectedBeltField()).ItemId;
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
						if (player.getHandInventoryField(GameGui.getNumberOfSelectedBeltField()).ItemAmount == 0)
						{
							player.setHandInventoryField(GameGui.getNumberOfSelectedBeltField(), ItemField(0, 0));
						}
						
						if (GameWorld.placeObject(objectPos, itemId, Objects.getContainer(),GameClock.getElapsedTime()))
						{
							ItemField temp = player.getHandInventoryField(GameGui.getNumberOfSelectedBeltField());
							temp -= 1;
							player.setHandInventoryField(GameGui.getNumberOfSelectedBeltField(), temp);
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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) || !GameGui.getIsEqGuiEnable())
	{
		if (GameGui.getHoldedItem().ItemId != 0)
		{
			LyingItems.pushNewItem(GameClock.getElapsedTime(),
				app.getMousePosInWorld(), GameGui.getHoldedItem());
			GameGui.clearHoldedItem();
		}
	}

	switch (last)
	{
	case mouseWheel::Up:
		GameGui.incrSelectedBeltField();
		break;
	case mouseWheel::Down:
		GameGui.decrSelectedBeltField();
		break;
	case mouseWheel::Stop:
		break;
	default:
		break;
	}

	GameGui.pushKeyState(key);
	GameGui.HudGui.pushNewValuesForHpInfo(200, static_cast<unsigned>(player.getHP()));
	GameGui.HudGui.pushNewValuesForMpInfo(200, static_cast<unsigned>(player.getMP()));
	pushChangesToGui();
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
	app.draw(GameGui.HudGui.getHudElement(false));
	app.draw(GameGui.HudGui.getHudElement(true));

	sf::Font font;
	font.loadFromFile("Data/Fonts/ariali.ttf");

	sf::Text amountItem;
	amountItem.setCharacterSize(16);
	amountItem.setFont(font);
	amountItem.setFillColor(sf::Color(255, 0, 0, 255));


	if (GameGui.getHoldedItem().ItemId != 0)
	{
		sf::RectangleShape holdedItemRep;
		holdedItemRep.setSize(sf::Vector2f(DefaultEqFieldSize, DefaultEqFieldSize));
		holdedItemRep.setTexture(mediaContainer.getTexture(GameGui.getHoldedItem().ItemId,
			TextureContainer::ItemsTextures));
		holdedItemRep.setPosition(app.getMousePosInWorld());
		amountItem.setString(std::to_string(GameGui.getHoldedItem().ItemAmount));
		amountItem.setPosition(app.getMousePosInWorld());

		app.draw(holdedItemRep);
		app.draw(amountItem);
	}

	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		app.draw(GameGui.HudGui.getBeltFieldRect(i));

		if (player.getHandInventoryField(i).ItemId != 0)
		{
			app.draw(GameGui.HudGui.getBeltFieldTextureRect(i));
			amountItem.setPosition(GameGui.HudGui.getBeltFieldRect(i).getPosition());
			amountItem.setString(std::to_string(player.getHandInventoryField(i).ItemAmount));
			app.draw(amountItem);
		}
	}

	if (!GameGui.getIsEqGuiEnable()) { return; }

	sf::Vector2u field;
	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		if (i < 3)
		{
			app.draw(GameGui.EquipmentGui.getArmorFieldRect(i));
			if (player.getArmorInventoryField(i).ItemId != 0)
			{
				app.draw(GameGui.EquipmentGui.getArmorTextureRect(i));
			}
		}
		field.x = i;
		for (size_t j = 0; j < PlayerFieldsNumber; j++)
		{
			field.y = j;

			app.draw(GameGui.EquipmentGui.getFieldRect(field));

			if (player.getInventoryField(field).ItemId != 0)
			{
				app.draw(GameGui.EquipmentGui.getTextureRect(field));
				amountItem.setPosition(GameGui.EquipmentGui.getFieldRect(field).getPosition());
				amountItem.setString(std::to_string(player.getInventoryField(field).ItemAmount));
				app.draw(amountItem);
			}
		}
	}
		
}

void Engine::DrawAll(IslandApp &app)
{
	drawWorld(app);
	drawPlayerGui(app);
	drawConsole(app);

	app.draw(*player.getShape());
}