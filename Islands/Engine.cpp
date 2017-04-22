#include "Engine.hpp"

void Engine::loadObjects()
{
	RawObjects.loadObjects();

	unsigned var = 1;
	for (auto & i : RawObjects.getObjects())
	{
		auto ref = const_cast<Object&>(i);
		ObjectsTextures.insert(std::pair<std::string, std::pair<unsigned, sf::Texture&>>(ref.getID(),
			std::pair<unsigned, sf::Texture&>(var, mediaContainer.pushObjectsTextures(RawObjects.getObjectsGraphicsFile(), ref.getTextureCord()))));
			var++;
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
	loadObjects();ErrorHandler::log("Load objects");

	player.set(&mediaContainer.PlayerTexture, sf::Vector2f(100, 100));
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

	if (!checkPlayerPos()) { ErrorHandler::log("player move above map"); }
}

void Engine::drawMap(IslandApp &app)
{
	sf::Vector2i PlayerPosToTile = Map::getTiledPosition(player.getCharacterCenterPosition());
	sf::RectangleShape TileShape;
	TileShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	
	for (int i = PlayerPosToTile.x - 30; i < PlayerPosToTile.x + 31; i++)
	{
		for (int j = PlayerPosToTile.y - 30; j < PlayerPosToTile.y + 31; j++)
		{
			if (i > -1 && j > -1)
			{
				TILE_TYPE tile = GameWorld.getTile(static_cast<sf::Vector2u>(sf::Vector2i(j, i)));
				TileShape.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(i, j))));
				switch (tile)
				{
				case TILE_TYPE::EMPTY:
					break;
				case TILE_TYPE::DIRT:
					TileShape.setTexture(&mediaContainer.TileTexture[0]);
					break;
				case TILE_TYPE::GRASS:
					break;
				case TILE_TYPE::ROCK:
					break;
				case TILE_TYPE::BRIGDE:
					TileShape.setTexture(&mediaContainer.TileTexture[10]);
					break;
				default:
					break;
				}
				if (tile != TILE_TYPE::EMPTY)
				{
					app.draw(TileShape);
				}
			}
		}
	}
}

void Engine::DrawAll(IslandApp &app)
{
	app.getIslandWindow()->setView(camera);

	drawMap(app);

	app.draw(*player.getShape());
}
