#include "Engine.hpp"
#include <noise/noise.h>

void Engine::generateMap()
{
	sf::Vector2f beg(0, 0);

	noise::module::Perlin noiseModule;
	noiseModule.SetSeed(static_cast<int>(time(time_t(NULL))));
	noiseModule.SetOctaveCount(6);
	noiseModule.SetFrequency(1.0);
	noiseModule.SetPersistence(0.25);

	for (size_t i = 0; i < 100; i++)
	{
		playerIsland.push_back(std::vector<Tile>(100));
		for (size_t j = 0; j < playerIsland[i].size(); j++)
		{
			double var = noiseModule.GetValue(1.25 + (0.1 * i), 0.75 + (0.1 * j), 0.5);

			if (var > 0.75)
			{
				playerIsland[i][j].set(TILE_TYPE::ROCK, sf::Vector2f(beg));
				playerIsland[i][j].setTexture(&mediaContainer.TextureContainer[0]);
			}
			else if(var > 0.5)
			{
				playerIsland[i][j].set(TILE_TYPE::DIRT, sf::Vector2f(beg));
				playerIsland[i][j].setTexture(&mediaContainer.TextureContainer[0]);
			}
			else if(var > 0.2)
			{
				playerIsland[i][j].set(TILE_TYPE::GRASS, sf::Vector2f(beg));
				playerIsland[i][j].setTexture(&mediaContainer.TextureContainer[0]);
			}
			else
			{
				playerIsland[i][j].set(TILE_TYPE::EMPTY, sf::Vector2f(beg));
			}

			beg.x += 64;
		}

		beg.y += 64;
		beg.x = 0;
	}

	for (size_t i = 0; i < playerIsland.size(); i++)
	{
		if (playerIsland[i].capacity() > playerIsland[i].size())
			playerIsland[i].shrink_to_fit();
	}
	playerIsland.shrink_to_fit();
}

Engine::~Engine()
{
	for (size_t i = 0; i < playerIsland.size(); i++)
	{
		playerIsland[i].clear();
	}
	objects.clear();
	mobs.clear();
}

void Engine::init()
{
	mediaContainer.load();
	player.set(&mediaContainer.TextureContainer[9], sf::Vector2f(100, 100));
	camera.setSize(sf::Vector2f(1280, 1024));

	generateMap();

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		camera.zoom(2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		camera.zoom(0.5);
	}

	player.move(movevctr);
	camera.setCenter(player.getPosition());
}

void Engine::DrawAll(sf::RenderWindow * window)
{
	window->setView(camera);

	for (size_t i = 0; i < playerIsland.size(); i++)
	{
		for (size_t j = 0; j < playerIsland[i].size(); j++)
		{
			if(playerIsland[i][j].getTileType() != TILE_TYPE::EMPTY)
			window->draw(*playerIsland[i][j].getShape());
		}
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		window->draw(*objects[i].getShape());
	}

	window->draw(*player.getShape());

	//draw clouds
}
