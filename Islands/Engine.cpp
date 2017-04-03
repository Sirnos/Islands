#include "Engine.hpp"

void Engine::checkPlayerBehaviour(sf::RenderWindow *window)
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

		if (spawnPoint.x > 0 && spawnPoint.y > 0 
			&& spawnPoint.x < MaxPosition * 64 && spawnPoint.y < MaxPosition * 64)
		{
			sf::Vector2i tile = Map::getTiledPosition(spawnPoint);

			if (GameMap.getTile(static_cast<sf::Vector2u>(sf::Vector2i(tile.y,tile.x)))->getTileType() != TILE_TYPE::EMPTY)
			{
				ErrorHandler::log(std::string("Spawn player position:"));
				ErrorHandler::log("Tile Y " + std::to_string(tile.y));
				ErrorHandler::log("Tile X " + std::to_string(tile.x));
				break;
			}
		}
	}
	player.setPosition(spawnPoint);
	player.setSpawnPoint(spawnPoint);
}

bool Engine::checkPlayerPos()
{
	sf::Vector2f playerPos = player.getCharacterCenterPosition();
	sf::Vector2i pos = Map::getTiledPosition(sf::Vector2f(playerPos.y,playerPos.x));

	
	if (pos.x < 0 || pos.y < 0 || pos.x > Map::MAP_SIZE || pos.y > Map::MAP_SIZE)
	{
		return false;
	}

	if (GameMap.getTile(sf::Vector2u(pos.x, pos.y))->getTileType() == TILE_TYPE::EMPTY)
	{
		return false;
	}
	
	return true;
}

Engine::~Engine()
{
	mobs.clear();
	ErrorHandler::log("Clear data");
}

void Engine::init()
{
	mediaContainer.load();
	player.set(&mediaContainer.PlayerTexture, sf::Vector2f(100, 100));
	camera.setSize(sf::Vector2f(1280, 1024));

	GameMap.generateMap();
	ErrorHandler::log("Generate map");
	ErrorHandler::log("Map Size " + std::to_string(Map::MAP_SIZE) + " x " + std::to_string(Map::MAP_SIZE));
	GameMap.fitMap();
	spawnPlayer();

}

void Engine::operator()(sf::RenderWindow * window)
{
	checkPlayerBehaviour(window);

	camera.setCenter(player.getCharacterCenterPosition());

	if (!checkPlayerPos())
	{
		ErrorHandler::log("player move above map");
	}
}

void Engine::drawMap(sf::RenderWindow *window)
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
				if (i < GameMap.getMapSize().x && j < GameMap.getMapSize().y)
				{
					if (GameMap.getTile(sf::Vector2u(j, i))->getTileType() != TILE_TYPE::EMPTY)
					{
						TileShape.setPosition(sf::Vector2f(Map::getNormalPosition(sf::Vector2i(i,j))));

						switch (GameMap.getTile(sf::Vector2u(j, i))->getTileType())
						{
						case TILE_TYPE::BRIGDE :
							TileShape.setTexture(&mediaContainer.TileTexture[10]);
							break;
						default:
							TileShape.setTexture(&mediaContainer.TileTexture[0]);
							break;
						}
						window->draw(TileShape);
					}
				}
			}
		}
	}
}

void Engine::DrawAll(sf::RenderWindow * window)
{
	window->setView(camera);

	drawMap(window);

	window->draw(*player.getShape());
}
