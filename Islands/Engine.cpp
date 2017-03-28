#include "Engine.hpp"
#include <SFML/Graphics/Texture.hpp>

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
	sf::Vector2f newCameraPos = player.getPosition();
	newCameraPos.x += (player.getSize().x / 2);
	newCameraPos.y += (player.getSize().y / 2);
	camera.setCenter(newCameraPos);
}

void Engine::drawMap(sf::RenderWindow *window)
{
	sf::Vector2f position = player.getPosition();
	position.x += player.getSize().x / 2;
	position.y += player.getSize().y / 2;

	sf::Vector2i PlayerPosToTile = GameMap.getTiledPosition(position);

	if (PlayerPosToTile.x == 0 && PlayerPosToTile.y == 0)
	{
		for (size_t i = 0; i < 21; i++)
		{
			for (size_t j = 0; j < 21; j++)
			{
				sf::Vector2u TileNumber(i, j);
				if (GameMap.getTile(TileNumber)->getTileType() != TILE_TYPE::EMPTY)
				{
					window->draw(*GameMap.getTile(TileNumber)->getShape());
				}
			}
		}
		return;
	}

	if (PlayerPosToTile.x == Map::MAP_SIZE && PlayerPosToTile.y == 0)
	{
		for (size_t i = PlayerPosToTile.x; i > PlayerPosToTile.x - 21; --i)
		{
			for (size_t j = 0; j < 21; j++)
			{
				sf::Vector2u TileNumber(i, j);
				if (GameMap.getTile(TileNumber)->getTileType() != TILE_TYPE::EMPTY)
				{
					window->draw(*GameMap.getTile(TileNumber)->getShape());
				}
			}
		}
		return;
	}

	if (PlayerPosToTile.x == 0 && PlayerPosToTile.y == Map::MAP_SIZE)
	{
		for (size_t i = 0; i < 21;i++)
		{
			for (size_t j = Map::MAP_SIZE; j > Map::MAP_SIZE - 21; --j)
			{
				sf::Vector2u TileNumber(i, j);
				if (GameMap.getTile(TileNumber)->getTileType() != TILE_TYPE::EMPTY)
				{
					window->draw(*GameMap.getTile(TileNumber)->getShape());
				}
			}
		}
		return;
	}

	if (PlayerPosToTile.x == Map::MAP_SIZE && PlayerPosToTile.y == Map::MAP_SIZE)
	{
		for (size_t i = Map::MAP_SIZE; i > Map::MAP_SIZE - 21; --i)
		{
			for (size_t j = Map::MAP_SIZE; j > Map::MAP_SIZE - 21; --j)
			{
				sf::Vector2u TileNumber(i, j);
				if (GameMap.getTile(TileNumber)->getTileType() != TILE_TYPE::EMPTY)
				{
					window->draw(*GameMap.getTile(TileNumber)->getShape());
				}
			}
		}
		return;
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
