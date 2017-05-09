#include "Engine.hpp"

void Engine::loadGameComponents()
{
	GameComponentsLoader loader;
	std::string graphicsfile;
	RawObjects.generateArray(loader.LoadObjectDefFromFile("Data/Objects.xml", graphicsfile));

	for (auto & i : RawObjects.getObjects())
	{
		auto ref = const_cast<Object&>(i);
			mediaContainer.pushTexture(TextureContainer::ObjectTextures,
				graphicsfile, ref.getTextureCord());
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
	TILE_TYPE tile = GameWorld.getTile(sf::Vector2u(tileIndex.y,tileIndex.x));
	if (tile == TILE_TYPE::EMPTY) { return; }
	shp.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(tileIndex.x,tileIndex.y))));
	switch (tile)
	{
	case TILE_TYPE::EMPTY:
		break;
	case TILE_TYPE::DIRT:
		shp.setTexture(&mediaContainer.getTexture(1,TextureContainer::TileTextures));
		break;
	case TILE_TYPE::GRASS:
		shp.setTexture(&mediaContainer.getTexture(2, TextureContainer::TileTextures));
		break;
	case TILE_TYPE::ROCK:
		shp.setTexture(&mediaContainer.getTexture(4, TextureContainer::TileTextures));
		break;
	case TILE_TYPE::BRIGDE:
		shp.setTexture(&mediaContainer.getTexture(2, TextureContainer::TileTextures));
		break;
	case TILE_TYPE::CLOUD:
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
	shp.setSize(sf::Vector2f(64, 64));
	//shp.setSize(RawObjects.getObject(ObjectID).getSize());
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
	ErrorHandler::log("Clear data");
}

void Engine::init()
{
	mediaContainer.load(); ErrorHandler::log("Load media");
	loadGameComponents(); ErrorHandler::log("Load game components");

	player.set(&mediaContainer.getTexture(1,TextureContainer::CharacterTextures), sf::Vector2f(100, 100));
	camera.setSize(sf::Vector2f(1280, 1024));

	GameWorld.init();
	ErrorHandler::log("Generate map");
	ErrorHandler::log("Map Size " + std::to_string(Map::MAP_SIZE) + " x " + std::to_string(Map::MAP_SIZE));
	spawnPlayer();
}

void Engine::operator()(IslandApp &app)
{
	checkPlayerBehaviour(app);

	camera.setCenter(player.getCharacterCenterPosition());

	if (!checkPlayerPos())
	{ 
		ErrorHandler::log("Player move above map");
		ErrorHandler::log("Pos:X" + std::to_string(player.getCharacterCenterPosition().x) +
			" :Y " + std::to_string(+player.getCharacterCenterPosition().y));
	}
}

void Engine::drawWorld(IslandApp & app)
{
	sf::Vector2i PlayerPosToTile = Map::getTiledPosition(player.getCharacterCenterPosition());
	sf::RectangleShape TileShape,
					 ObjectShape;
	TileShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));

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

void Engine::DrawAll(IslandApp &app)
{
	app.getIslandWindow()->setView(camera);

	drawWorld(app);

	app.draw(*player.getShape());
}