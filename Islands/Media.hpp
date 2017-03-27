#pragma once

#include <SFML/Graphics.hpp>

const int size = 10;
const int pos = 10;

struct Media
{
	sf::Image rawImage;

	std::vector <sf::Texture> TextureContainer;

	std::vector <sf::Texture> ObjectsTexture;
	std::vector <sf::Texture> ItemsTexture;

	void load()
	{
		rawImage.loadFromFile("sprites.png");
		for (size_t i = 0; i < 12; i++)
		{
			TextureContainer.push_back(sf::Texture());
		}
		TextureContainer[0].loadFromImage(rawImage, sf::IntRect(pos, pos, size, size));//normal
		TextureContainer[1].loadFromImage(rawImage, sf::IntRect(0, pos, size, size));//back left
		TextureContainer[2].loadFromImage(rawImage, sf::IntRect(0, pos * 2, size, size));//up-left corner
		TextureContainer[3].loadFromImage(rawImage, sf::IntRect(pos, pos *2, size, size));//up
		TextureContainer[4].loadFromImage(rawImage, sf::IntRect(pos *2, pos * 2, size, size));// up right corner
		TextureContainer[5].loadFromImage(rawImage, sf::IntRect(pos * 2, pos *1, size, size));//back right
		TextureContainer[6].loadFromImage(rawImage, sf::IntRect(pos*1, 0, size, size));// down
		TextureContainer[7].loadFromImage(rawImage, sf::IntRect(0, 0, size, size));//down left corner
		TextureContainer[8].loadFromImage(rawImage, sf::IntRect(pos * 2, 0, size, size));//down right corner
		TextureContainer[9].loadFromFile("char.png", sf::IntRect(0,0,40,60));//player
		TextureContainer[10].loadFromImage(rawImage, sf::IntRect(0, pos * 3, size, size));
	}

	~Media()
	{
		TextureContainer.clear();
	}
};