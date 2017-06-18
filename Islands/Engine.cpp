#include "Engine.hpp"

void Engine::loadGameComponents()
{
	GameComponentsLoader loader;
	std::string graphicsfile;
	RawObjects.generateArray(loader.LoadObjectDefFromFile(graphicsfile));

	for (auto & i : RawObjects.getObjects())
	{
		auto ref = const_cast<ObjectDef&>(i);
			mediaContainer.pushTexture(TextureContainer::ObjectTextures,
				graphicsfile, ref.getTextureCord());
			mediaContainer.pushTexture(TextureContainer::ItemsTextures,
				graphicsfile, ref.getTextureCord());
	}
	loader.GenerateItemsFromObjectDef(RawObjects.getObjects(), Items);

	for (auto & i : Items.getContainer())
	{
			ErrorHandler::log("Load Item: "+i->getName()+" maxStack: 64");
	}

	std::string itemTextureFile;
	std::vector<sf::IntRect> ItemTextRect;
	loader.LoadItemDefFromFile(Items.getContainer(), itemTextureFile, ItemTextRect);

	for (auto & i : ItemTextRect)
	{
		mediaContainer.pushTexture(TextureContainer::ItemsTextures, itemTextureFile, i);
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
	const int MaxPosition = Map::MAP_SIZE - 10;

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
		//for armor inventory
		if (i < 3)
		{
			GameGui.EquipmentGui.getArmorFieldRect(i).setFillColor(DefaultEqFieldColor);
			GameGui.EquipmentGui.getArmorTextureRect(i).setTexture(nullptr);

			if (GameGui.EquipmentGui.getHoverFromArmorField(i))
			{
				GameGui.EquipmentGui.getArmorFieldRect(i).setFillColor(EqFieldColorWhenIsSelected);
			}

			unsigned ArmorItemId = player.getArmorInventoryField(i).ItemId;
			if (ArmorItemId != 0)
			{
				GameGui.EquipmentGui.getArmorTextureRect(i).setTexture(&mediaContainer.getTexture(ArmorItemId,
					TextureContainer::ItemsTextures));
			}
		}

		//for belt inventory
		GameGui.HudGui.getBeltFieldRect(i).setFillColor(DefaultEqFieldColor);
		GameGui.HudGui.getBeltFieldTextureRect(i).setTexture(nullptr);
		if (i == GameGui.getNumberOfSelectedBeltField())
		{
			GameGui.HudGui.getBeltFieldRect(i).setFillColor(EqFieldColorWhenIsSelected);
		}

		unsigned itemId = player.getHandInventoryField(i).ItemId;

		if (itemId != 0)
		{
			GameGui.HudGui.getBeltFieldTextureRect(i).setTexture(&mediaContainer.
				getTexture(itemId,TextureContainer::ItemsTextures));
		}

		//for normal inventory
		for (size_t j = 0; j < PlayerFieldsNumber; j++)
		{
			GameGui.EquipmentGui.getFieldRect(sf::Vector2u(i,j)).setFillColor(DefaultEqFieldColor);
			GameGui.EquipmentGui.getTextureRect(sf::Vector2u(i, j)).setTexture(nullptr);

			if (GameGui.EquipmentGui.getHoverFromInventoryField(sf::Vector2u(i,j)))
			{
				GameGui.EquipmentGui.getFieldRect(sf::Vector2u(i, j)).setFillColor(EqFieldColorWhenIsSelected);
			}

			unsigned tempItemId = player.getInventoryField(sf::Vector2u(i, j)).ItemId;
			if (tempItemId != 0)
			{
				GameGui.EquipmentGui.getTextureRect(sf::Vector2u(i, j)).setTexture(&mediaContainer.getTexture(tempItemId,
					TextureContainer::ItemsTextures));
			}
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
		shp.setTexture(&mediaContainer.getTexture(1,TextureContainer::TileTextures));
		break;
	case TILE::GRASS:
		shp.setTexture(&mediaContainer.getTexture(2, TextureContainer::TileTextures));
		break;
	case TILE::ROCK:
		shp.setTexture(&mediaContainer.getTexture(4, TextureContainer::TileTextures));
		break;
	case TILE::BRIGDE:
		shp.setTexture(&mediaContainer.getTexture(2, TextureContainer::TileTextures));
		break;
	case TILE::CLOUD:
		shp.setTexture(&mediaContainer.getTexture(6, TextureContainer::TileTextures));
		break;
	default:
		break;
	}
	window.draw(shp);
}

void Engine::drawObject(sf::Vector2u objectIndex, sf::RenderWindow & window, sf::RectangleShape &shp)
{
	unsigned ObjectID = GameWorld.getObject(sf::Vector2u(objectIndex.y,objectIndex.x));
	if (ObjectID == 0) { return; }
	if (ObjectID > RawObjects.getObjects().size()) { return; }
	shp.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(objectIndex.x, objectIndex.y))));
	shp.setTexture(&mediaContainer.getTexture(ObjectID, TextureContainer::ObjectTextures));
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
	GameComponentUnloader unloader;
	unloader.clearItems(Items.getContainer());
	ErrorHandler::log("Clear data");
}

void Engine::init()
{
	mediaContainer.load(); ErrorHandler::log("Load media");
	loadGameComponents(); ErrorHandler::log("Load game components");

	player.set(&mediaContainer.getTexture(1,TextureContainer::CharacterTextures), sf::Vector2f(100, 100),sf::Vector2f(10,8));
	camera.setSize(sf::Vector2f(1280, 1024));

	GameWorld.init();
	ErrorHandler::log("Generate map");
	ErrorHandler::log("Map Size " + std::to_string(Map::MAP_SIZE) + " x " + std::to_string(Map::MAP_SIZE));
	spawnPlayer();

	GameGui.create();
}

void Engine::operator()(IslandApp &app,char key,mouseWheel last, bool isMouseClick)
{
	checkPlayerBehaviour(app);

	camera.setCenter(player.getCharacterCenterPosition());

	auto Window = app.getIslandWindow();

	GameGui.setNewPosition(Window->mapPixelToCoords(GameGui.EquipmentGui.defaultEquipmentGuiPosOnScreen));
	GameGui.HudGui.setNewPosition(Window->mapPixelToCoords(GameGui.HudGui.HpInfoScreenPos),
		Window->mapPixelToCoords(GameGui.HudGui.MpInfoScreenPos),Window->mapPixelToCoords(GameGui.HudGui.BeltFieldPosOnScreen));

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

				if (mousePosInWorld.x > fieldPos.x && mousePosInWorld.x < fieldPos.x + DefaultEqFieldSize
					&& mousePosInWorld.y > fieldPos.y && mousePosInWorld.y < fieldPos.y + DefaultEqFieldSize)
				{
					GameGui.EquipmentGui.setHoverForArmorField(i, true);
					if (isMouseClick)
					{
						if (GameGui.getHoldedItem().ItemId != 0)
						{
							if (player.getArmorInventoryField(i).ItemId != 0)
							{
								ItemField temp(GameGui.getHoldedItem());
								GameGui.assingNewItemToHold(player.getArmorInventoryField(i));
								player.setArmorField(i, temp);
							}
							else
							{
								player.setArmorField(i, GameGui.getHoldedItem());
								GameGui.clearHoldedItem();
							}
						}
						else
						{
							GameGui.assingNewItemToHold(player.getArmorInventoryField(i));
							player.setArmorField(i, ItemField());
						}
					}
				}
			}

			for (size_t j = 0; j < PlayerFieldsNumber; j++)
			{
				if (GameGui.EquipmentGui.getHoverFromInventoryField(sf::Vector2u(i, j)))
				{
					GameGui.EquipmentGui.setHoverForInventoryField(sf::Vector2u(i, j), false);
				}

				sf::Vector2f fieldPos = GameGui.EquipmentGui.getFieldRect(sf::Vector2u(i, j)).getPosition();
				if (mousePosInWorld.x > fieldPos.x && mousePosInWorld.x < fieldPos.x + DefaultEqFieldSize
					&& mousePosInWorld.y > fieldPos.y && mousePosInWorld.y < fieldPos.y + DefaultEqFieldSize)
				{
					GameGui.EquipmentGui.setHoverForInventoryField(sf::Vector2u(i, j), true);
					if (isMouseClick)
					{
						if (GameGui.getHoldedItem().ItemId != 0)
						{
							if (player.getInventoryField(sf::Vector2u(i,j)).ItemId != 0)
							{
								ItemField temp(GameGui.getHoldedItem());
								GameGui.assingNewItemToHold(player.getInventoryField(sf::Vector2u(i, j)));
								player.setArmorField(i, temp);
							}
							else
							{
								player.setInventoryField(sf::Vector2u(i, j), GameGui.getHoldedItem());
								GameGui.clearHoldedItem();
							}
						}
						else
						{
							GameGui.assingNewItemToHold(player.getInventoryField(sf::Vector2u(i, j)));
							player.setInventoryField(sf::Vector2u(i, j), ItemField());
						}
					}
				}
			}
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
					 ObjectShape;
	TileShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	ObjectShape.setSize(RawObjects.ObjectGraphicsSize);

	for (int i = PlayerPosToTile.x - 30; i < PlayerPosToTile.x + 31; i++)
	{
		for (int j = PlayerPosToTile.y - 30; j < PlayerPosToTile.y + 31; j++)
		{
			if (j < 0 && i < 0) { continue; }
			drawTile(static_cast<sf::Vector2u>(sf::Vector2i(i, j)), *app.getIslandWindow(), TileShape);
			drawObject(static_cast<sf::Vector2u>(sf::Vector2i(i, j)), *app.getIslandWindow(), TileShape);
		}
	}
}

void Engine::drawPlayerGui(IslandApp & app)
{
	app.draw(*GameGui.HudGui.getHudElement(false));
	app.draw(*GameGui.HudGui.getHudElement(true));

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
		holdedItemRep.setTexture(&mediaContainer.getTexture(GameGui.getHoldedItem().ItemId,
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
	app.getIslandWindow()->setView(camera);

	drawWorld(app);

	drawPlayerGui(app);

	app.draw(*player.getShape());

}