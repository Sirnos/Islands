#pragma once

#include <SFML/Graphics.hpp>
#include "Log.hpp"


struct Media
{
	const int size = 10;
	const int pos = 10;

	sf::Image TileImage;

	std::vector <sf::Texture> TileTexture;
	std::vector <sf::Texture> ObjectsTexture;
	std::vector <sf::Texture> ItemsTexture;
	std::vector <sf::Texture> GUITexture;
	
	sf::Texture PlayerTexture;

	void load()
	{
		if (TileImage.loadFromFile("Tiles.png"))
		{
			ErrorHandler::log("Textures loaded");
		}
		else
			ErrorHandler::log("Textures not loaded");
		
		TileTexture.resize(12);

		TileTexture[0].loadFromImage(TileImage, sf::IntRect(pos, pos, size, size));//normal
		TileTexture[1].loadFromImage(TileImage, sf::IntRect(0, pos, size, size));//back left
		TileTexture[2].loadFromImage(TileImage, sf::IntRect(0, pos * 2, size, size));//up-left corner
		TileTexture[3].loadFromImage(TileImage, sf::IntRect(pos, pos * 2, size, size));//up
		TileTexture[4].loadFromImage(TileImage, sf::IntRect(pos * 2, pos * 2, size, size));// up right corner
		TileTexture[5].loadFromImage(TileImage, sf::IntRect(pos * 2, pos *1, size, size));//back right
		TileTexture[6].loadFromImage(TileImage, sf::IntRect(pos * 1, 0, size, size));// down
		TileTexture[7].loadFromImage(TileImage, sf::IntRect(0, 0, size, size));//down left corner
		TileTexture[8].loadFromImage(TileImage, sf::IntRect(pos * 2, 0, size, size));//down right corner
		TileTexture[9].loadFromImage(TileImage, sf::IntRect(0, pos * 3, size, size));
		TileTexture[10].loadFromImage(TileImage, sf::IntRect(pos, pos * 3, size, size));//brigde

		ObjectsTexture.resize(10);
		ObjectsTexture[0].loadFromImage(TileImage, sf::IntRect(0, pos * 3, size, size));//shrub
		ObjectsTexture[1].loadFromImage(TileImage, sf::IntRect(pos * 2, pos * 3, size, size));//small stone
		ObjectsTexture[2].loadFromImage(TileImage, sf::IntRect(pos * 3, pos * 3, size, size));//stone
		ObjectsTexture[3].loadFromImage(TileImage, sf::IntRect(pos * 4, pos * 3, size, size));//big-stone
		ObjectsTexture[4].loadFromImage(TileImage, sf::IntRect(pos * 5, pos * 3, size, size));//flint
		ObjectsTexture[5].loadFromImage(TileImage, sf::IntRect(pos * 6, pos * 3, size, size));//leafpile
		ObjectsTexture[6].loadFromImage(TileImage, sf::IntRect(pos * 7, pos * 3, size, size));//flower
		ObjectsTexture[7].loadFromImage(TileImage, sf::IntRect(pos * 8, pos * 3, size, size));//vegetable


		PlayerTexture.loadFromFile("char.png", sf::IntRect(0, 0, 40, 60));
	}

	~Media()
	{
		GUITexture.clear();
		ObjectsTexture.clear();
		ItemsTexture.clear();
		TileTexture.clear();
	}
};