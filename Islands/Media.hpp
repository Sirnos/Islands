#pragma once

#include <SFML/Graphics.hpp>
#include "Log.hpp"


struct Media
{
	const int size = 10;
	const int pos = 10;

	sf::Image rawImage;

	std::vector <sf::Texture> TileTexture;
	std::vector <sf::Texture> ObjectsTexture;
	std::vector <sf::Texture> ItemsTexture;
	
	sf::Texture PlayerTexture;

	void load()
	{
		if (rawImage.loadFromFile("sprites.png"))
		{
			ErrorHandler::log("Textures loaded");
		}
		else
			ErrorHandler::log("Textures not loaded");
		
		TileTexture.resize(12);

		TileTexture[0].loadFromImage(rawImage, sf::IntRect(pos, pos, size, size));//normal
		TileTexture[1].loadFromImage(rawImage, sf::IntRect(0, pos, size, size));//back left
		TileTexture[2].loadFromImage(rawImage, sf::IntRect(0, pos * 2, size, size));//up-left corner
		TileTexture[3].loadFromImage(rawImage, sf::IntRect(pos, pos *2, size, size));//up
		TileTexture[4].loadFromImage(rawImage, sf::IntRect(pos *2, pos * 2, size, size));// up right corner
		TileTexture[5].loadFromImage(rawImage, sf::IntRect(pos * 2, pos *1, size, size));//back right
		TileTexture[6].loadFromImage(rawImage, sf::IntRect(pos*1, 0, size, size));// down
		TileTexture[7].loadFromImage(rawImage, sf::IntRect(0, 0, size, size));//down left corner
		TileTexture[8].loadFromImage(rawImage, sf::IntRect(pos * 2, 0, size, size));//down right corner
		TileTexture[9].loadFromImage(rawImage, sf::IntRect(0, pos * 3, size, size));
		TileTexture[10].loadFromImage(rawImage, sf::IntRect(pos, pos * 3, size, size));//brigde

		PlayerTexture.loadFromFile("char.png", sf::IntRect(0, 0, 40, 60));
	}

	~Media()
	{
		ObjectsTexture.clear();
		ItemsTexture.clear();
		TileTexture.clear();
	}
};