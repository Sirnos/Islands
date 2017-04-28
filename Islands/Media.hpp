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
	std::vector<sf::Texture> ItemsTexture;

	void load()
	{
		TileImage.loadFromFile("Data/Tiles.png");

		TileTexture.resize(12);

		TileTexture[0].loadFromImage(TileImage, sf::IntRect(pos, pos, size, size));//normal

		TileTexture[1].loadFromImage(TileImage, sf::IntRect(pos, pos * 3, size, size));//brigde

		PlayerTexture.loadFromFile("Data/char.png", sf::IntRect(0, 0, 40, 60));
	}
	sf::Texture& pushObjectsTextures(std::string fileName,sf::IntRect textCord)
	{
		ObjectsTexture.push_back(sf::Texture());
		if (!ObjectsTexture.back().loadFromFile(fileName, textCord))
		{
			ErrorHandler::log("Object texture load incorectly");
		}
		return ObjectsTexture.back();
	}

	Media()
	{
		ObjectsTexture.push_back(sf::Texture());
		ItemsTexture.push_back(sf::Texture());
	}
	~Media()
	{
		ObjectsTexture.clear();
		ItemsTexture.clear();
		TileTexture.clear();
	}
};