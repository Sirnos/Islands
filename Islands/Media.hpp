#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "Log.hpp"


struct Media
{
	const int size = 10;
	const int pos = 10;

	sf::Image TileImage;

	std::vector <sf::Texture> TileTexture;
	
	sf::Texture PlayerTexture;
	std::vector<sf::Texture> ObjectsTexture;
	std::vector<std::pair<std::string, sf::Texture>> ItemsTexture;

	void load()
	{
		TileImage.loadFromFile("Data/Tiles.png");

		TileTexture.resize(12);

		TileTexture[0].loadFromImage(TileImage, sf::IntRect(pos, pos, size, size));//normal
		/*
		TileTexture[1].loadFromImage(TileImage, sf::IntRect(0, pos, size, size));//back left
		TileTexture[2].loadFromImage(TileImage, sf::IntRect(0, pos * 2, size, size));//up-left corner
		TileTexture[3].loadFromImage(TileImage, sf::IntRect(pos, pos * 2, size, size));//up
		TileTexture[4].loadFromImage(TileImage, sf::IntRect(pos * 2, pos * 2, size, size));// up right corner
		TileTexture[5].loadFromImage(TileImage, sf::IntRect(pos * 2, pos *1, size, size));//back right
		TileTexture[6].loadFromImage(TileImage, sf::IntRect(pos * 1, 0, size, size));// down
		TileTexture[7].loadFromImage(TileImage, sf::IntRect(0, 0, size, size));//down left corner
		TileTexture[8].loadFromImage(TileImage, sf::IntRect(pos * 2, 0, size, size));//down right corner
		TileTexture[9].loadFromImage(TileImage, sf::IntRect(0, pos * 3, size, size));
		*/
		TileTexture[10].loadFromImage(TileImage, sf::IntRect(pos, pos * 3, size, size));//brigde

		PlayerTexture.loadFromFile("Data/char.png", sf::IntRect(0, 0, 40, 60));
	}
	void pushObjectsTextures(std::string fileName,sf::IntRect textCord)
	{
		sf::Texture temp;
		temp.loadFromFile(fileName, textCord);
		ObjectsTexture.push_back(sf::Texture(temp));
	}

	~Media()
	{
		ObjectsTexture.clear();
		ItemsTexture.clear();
		TileTexture.clear();
	}
};