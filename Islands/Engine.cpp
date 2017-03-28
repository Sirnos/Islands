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
	
	for (int i = PlayerPosToTile.x - 30; i < PlayerPosToTile.x + 31; i++)
	{
		for (int j = PlayerPosToTile.y - 30; j < PlayerPosToTile.y + 31; j++)
		{
			if (i > -1 && j > -1)
			{
				if (i < 100 && j < 100)
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
