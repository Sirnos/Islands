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
	GameGui.EquipmentGui.pushPlayerArmorInventory(player.getArmorInv());
	GameGui.HudGui.pushPlayerBeltInventory(player.getBelt());

	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		for (size_t j = 0; j < PlayerFieldsNumber; j++)
		{
			GameGui.EquipmentGui.setEqField(sf::Vector2u(i, j), player.getInventoryField(sf::Vector2u(i, j)));

			if (GameGui.EquipmentGui.getItemField(sf::Vector2u(i,j)).ItemId != 0)
			{
				GameGui.EquipmentGui.pushTextureToFields(sf::Vector2u(i, j), &mediaContainer.getTexture(
					GameGui.EquipmentGui.getItemField(sf::Vector2u(i,j)).ItemId, TextureContainer::ItemsTextures));
			}
		}
	}
}

void Engine::operator()(IslandApp &app,char key)
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
	GameGui.pushKeyState(key);
	GameGui.HudGui.pushNewValuesForHpInfo(200, static_cast<unsigned>(player.getHP()));
	GameGui.HudGui.pushNewValuesForMpInfo(200, static_cast<unsigned>(player.getMP()));
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

	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		app.draw(GameGui.HudGui.getFieldFromBelt(i)->FieldRect);
	}

	if (!GameGui.getIsEqGuiEnable()) { return; }
	sf::Font font;
	font.loadFromFile("Data/Fonts/ariali.ttf");

	sf::Text amountItem;
	amountItem.setCharacterSize(16);
	amountItem.setFont(font);
	amountItem.setFillColor(sf::Color(255, 0, 0, 255));

	sf::Vector2u field;
	for (size_t i = 0; i < PlayerFieldsNumber; i++)
	{
		if (i < 3)
		{
			app.draw(GameGui.EquipmentGui.getArmorFieldRect(i));
		}
		field.x = i;
		for (size_t j = 0; j < PlayerFieldsNumber; j++)
		{
			field.y = j;
			app.draw(GameGui.EquipmentGui.getFieldRect(field));
			amountItem.setPosition(GameGui.EquipmentGui.getFieldRect(field).getPosition());
			amountItem.setString(std::to_string(GameGui.EquipmentGui.getItemField(field).ItemAmount));
			if (GameGui.EquipmentGui.getItemField(field).ItemId != 0)
			{
				app.draw(GameGui.EquipmentGui.getTextureRect(field));
			}
			app.draw(amountItem);
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