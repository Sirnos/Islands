#include "Engine.hpp"

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
	sf::Vector2f beg(0, 0);
	mediaContainer.load();

	player.set(&mediaContainer.TextureContainer[9], sf::Vector2f(100, 100));

	for (size_t i = 0; i < 40; i++)
	{
		playerIsland.push_back(std::vector<Tile>(40));
		for (size_t j = 0; j < playerIsland[i].size(); j++)
		{
			playerIsland[i][j].set(TILE_TYPE::GRASS, sf::Vector2f(beg));

			if (j < 10 && i < 10)
			{
				playerIsland[i][j].setTexture(&mediaContainer.TextureContainer[0]);
			}
			else
			{
				playerIsland[i][j].setTexture(&mediaContainer.TextureContainer[8]);
			}
			beg.x += 64;
		}
		beg.y += 64;
		beg.x = 0;
	}

	for (size_t i = 0; i < 10; i++)
	{
		playerIsland[i][0].setTexture(&mediaContainer.TextureContainer[1]);
		playerIsland[i][10].setTexture(&mediaContainer.TextureContainer[5]);
	}
	for (size_t i = 0; i < 10; i++)
	{
		playerIsland[0][i].setTexture(&mediaContainer.TextureContainer[6]);
		playerIsland[9][i].setTexture(&mediaContainer.TextureContainer[3]);
	}

	playerIsland[0][0].setTexture(&mediaContainer.TextureContainer[7]);
	playerIsland[9][0].setTexture(&mediaContainer.TextureContainer[2]);
	playerIsland[0][10].setTexture(&mediaContainer.TextureContainer[8]);

	for (size_t i = 0; i < playerIsland.size(); i++)
	{
		if(playerIsland[i].capacity() > playerIsland[i].size())
			playerIsland[i].shrink_to_fit();
	}
	playerIsland.shrink_to_fit();

	camera.setSize(sf::Vector2f(1280, 1024));
}

void Engine::operator()()
{
	sf::Vector2f movevctr;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movevctr.x -= 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movevctr.x += 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movevctr.y -= 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movevctr.y += 3;
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
