#include "Engine.hpp"
#include <SFML/Graphics/Texture.hpp>

void Engine::spawnPlayer()
{
	srand(static_cast<unsigned int>(time(NULL)));
	sf::Vector2f spawnPoint;

	while (true)
	{
		spawnPoint = sf::Vector2f(rand() % 250 * 64, rand() % 250 * 64);

		if (spawnPoint.x > 0 && spawnPoint.y > 0 && spawnPoint.x < 250 * 64 && spawnPoint.y < 250 * 64)
		{
			sf::Vector2i tile = GameMap.getTiledPosition(spawnPoint);

			if (GameMap.getTile(static_cast<sf::Vector2u>(tile))->getTileType() != TILE_TYPE::EMPTY)
			{
				break;
			}
		}
	}
	player.setPosition(spawnPoint);
}

bool Engine::checkPlayerPos()
{
	sf::Vector2f playerPos = player.getCharacterCenterPosition();
	sf::Vector2i pos = GameMap.getTiledPosition(playerPos);

	/*
	if (pos.x < 0 || pos.y < 0 || pos.x > Map::MAP_SIZE || pos.y > Map::MAP_SIZE)
	{
		return false;
	}

	if (GameMap.getTile(sf::Vector2u(pos.x, pos.y))->getTileType() == TILE_TYPE::EMPTY)
	{
		return false;
	}
	*/
	return true;
}

Engine::~Engine()
{
	objects.clear();
	mobs.clear();
}

void Engine::init()
{
	mediaContainer.load();
	player.set(&mediaContainer.TextureContainer[9], sf::Vector2f(100, 100));
	camera.setSize(sf::Vector2f(1280, 1024));

	GameMap.generateMap();
	GameMap.fitMap();
	GameMap.bindTexturesToTiles(mediaContainer.TextureContainer.data(),mediaContainer.TextureContainer.size());

	spawnPlayer();

	objects.resize(10);
	for (size_t i = 0; i < 10; i++)
	{
		objects[i].setTexture(&mediaContainer.TextureContainer[10]);
		objects[i].setSize(sf::Vector2f(40, 40));
		objects[i].setPosition(sf::Vector2f(64 * i * 2, 64 * i * 2));
	}
	objects.shrink_to_fit();
}

void Engine::operator()()
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movevctr.y -= 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movevctr.y += 5;
	}

	player.move(movevctr);
	sf::Vector2f newCameraPos = player.getCharacterCenterPosition();
	camera.setCenter(newCameraPos);

	if (!checkPlayerPos())
	{
		exit(0);
	}
}

void Engine::drawMap(sf::RenderWindow *window)
{
	sf::Vector2i PlayerPosToTile = GameMap.getTiledPosition(player.getCharacterCenterPosition());
	
	for (int i = PlayerPosToTile.x - 30; i < PlayerPosToTile.x + 31; i++)
	{
		for (int j = PlayerPosToTile.y - 30; j < PlayerPosToTile.y + 31; j++)
		{
			if (i > -1 && j > -1)
			{
				if (i < GameMap.getMapSize().x && j < GameMap.getMapSize().y)
				{
					if(GameMap.getTile(sf::Vector2u(j, i))->getTileType() != TILE_TYPE::EMPTY)
						window->draw(*GameMap.getTile(sf::Vector2u(j, i))->getShape());
				}
			}
		}
	}

}

void Engine::DrawAll(sf::RenderWindow * window)
{
	window->setView(camera);

	drawMap(window);

	for (size_t i = 0; i < objects.size(); i++)
	{
		window->draw(*objects[i].getShape());
	}

	window->draw(*player.getShape());

}
