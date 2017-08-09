#pragma once

#include<SFML/Graphics/Texture.hpp>
#include "ErrorHandler.hpp"

enum class TextureContainer : unsigned
{
	TerrainTextures,
	CharacterTextures,
	ObjectTextures,
	ItemsTextures,
};

class Media
{
	int size = 16;
	int pos = 16;

	sf::Image TileImage;

	std::vector <sf::Texture> TerrainTexture;
	
	std::vector<sf::Texture> CharacterTexture;
	std::vector<sf::Texture> ObjectsTexture;
	std::vector<sf::Texture> ItemsTexture;

public:
	void load()
	{
		TileImage.loadFromFile("Data/Objects.png");

		TerrainTexture.resize(12);

		TerrainTexture[1].loadFromImage(TileImage, sf::IntRect(0, 0, size, size));//dirt
		TerrainTexture[2].loadFromImage(TileImage, sf::IntRect(pos,0, size, size));//grass
		TerrainTexture[3].loadFromImage(TileImage, sf::IntRect(pos * 2, 0, size, size));//sand
		TerrainTexture[4].loadFromImage(TileImage, sf::IntRect(pos * 3, 0, size, size));//rocks
		TerrainTexture[5].loadFromImage(TileImage, sf::IntRect(pos * 4, 0, size, size));//water
		TerrainTexture[6].loadFromImage(TileImage, sf::IntRect(pos * 5, 0, size, size));//cloud

		CharacterTexture.push_back(sf::Texture());
		CharacterTexture.back().loadFromFile("Data/char.png", sf::IntRect(0, 0, 40, 60));
	}

	void pushTexture(TextureContainer destinationContainer, std::string fileName, sf::IntRect textCord)
	{
		switch (destinationContainer)
		{
		case TextureContainer::TerrainTextures:
			TerrainTexture.push_back(sf::Texture());
			if (!TerrainTexture.back().loadFromFile(fileName, textCord))
			{
				ErrorHandler::log("Tile texture load incorectly");
			}
			break;
		case TextureContainer::CharacterTextures:
			CharacterTexture.push_back(sf::Texture());
			if (!CharacterTexture.back().loadFromFile(fileName, textCord))
			{
				ErrorHandler::log("Character texture load incorectly");
			}
			break;
		case TextureContainer::ObjectTextures:
			ObjectsTexture.push_back(sf::Texture());
			if (!ObjectsTexture.back().loadFromFile(fileName, textCord))
			{
				ErrorHandler::log("Object texture load incorectly");
			}
			break;
		case TextureContainer::ItemsTextures:
			ItemsTexture.push_back(sf::Texture());
			if (!ItemsTexture.back().loadFromFile(fileName, textCord))
			{
				ErrorHandler::log("Item texture load incorectly");
			}
			break;
		}
	}
	sf::Texture* getTexture(size_t index, TextureContainer typeOfContainer)
	{
		switch (typeOfContainer)
		{
		case TextureContainer::TerrainTextures:
			return &TerrainTexture[index];
			break;
		case TextureContainer::CharacterTextures:
			return &CharacterTexture[index];
			break;
		case TextureContainer::ObjectTextures:
			return &ObjectsTexture[index];
			break;
		case TextureContainer::ItemsTextures:
			return &ItemsTexture[index];
			break;
		}
		return nullptr;
	}

	size_t getSizeOfContainer(TextureContainer typeOfContainer)
	{
		switch (typeOfContainer)
		{
		case TextureContainer::TerrainTextures:
			return TerrainTexture.size();
			break;
		case TextureContainer::CharacterTextures:
			return CharacterTexture.size();
			break;
		case TextureContainer::ObjectTextures:
			return ObjectsTexture.size();
			break;
		case TextureContainer::ItemsTextures:
			return ItemsTexture.size();
			break;
		}
	}

	Media()
	{
		TerrainTexture.push_back(sf::Texture());
		CharacterTexture.push_back(sf::Texture());
	}
	~Media()
	{
		ObjectsTexture.clear();
		ItemsTexture.clear();
		TerrainTexture.clear();
		CharacterTexture.clear();
	}
};