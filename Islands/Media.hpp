#pragma once

#include <SFML/Graphics.hpp>

struct Media
{
	sf::Image rawImage;

	std::vector <sf::Texture> TextureContainer;

	void load()
	{
		rawImage.loadFromFile("overworld.png");
		for (size_t i = 0; i < 12; i++)
		{
			TextureContainer.push_back(sf::Texture());
		}
		TextureContainer[0].loadFromImage(rawImage, sf::IntRect(0, 50, 10, 10));//normal
		TextureContainer[1].loadFromImage(rawImage, sf::IntRect(0, 20, 10, 10));//back left
		TextureContainer[2].loadFromImage(rawImage, sf::IntRect(0, 30, 10, 10));//up-left corner
		TextureContainer[3].loadFromImage(rawImage, sf::IntRect(10, 30, 10, 10));//up
		TextureContainer[4].loadFromImage(rawImage, sf::IntRect(40, 30, 10, 10));//right corner
		TextureContainer[5].loadFromImage(rawImage, sf::IntRect(40, 20, 10, 10));//back right
		TextureContainer[6].loadFromImage(rawImage, sf::IntRect(30, 0, 10, 10));// down
		TextureContainer[7].loadFromImage(rawImage, sf::IntRect(0, 0, 10, 10));//down left corner
		TextureContainer[8].loadFromImage(rawImage, sf::IntRect(60,130,10,10));//water
		TextureContainer[9].loadFromFile("char.png", sf::IntRect(0,0,40,60));
	}

	~Media()
	{
		TextureContainer.clear();
	}
};