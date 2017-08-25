#pragma once

#include <SFML/Graphics/Texture.hpp>
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

	bool pushTexture(TextureContainer container, std::string fileName, sf::IntRect textCord)
	{
		sf::Texture texture;
		bool isSuccefull = false;
		isSuccefull = texture.loadFromFile(fileName, textCord);
		
		switch (container)
		{
		case TextureContainer::TerrainTextures:
			TerrainTexture.push_back(texture);
			break;
		case TextureContainer::CharacterTextures:
			CharacterTexture.push_back(texture);
			break;
		case TextureContainer::ObjectTextures:
			ObjectsTexture.push_back(texture);
			break;
		case TextureContainer::ItemsTextures:
			ItemsTexture.push_back(texture);
			break;
		default:
			break;
		}
		return isSuccefull;
	}
	void pushTextures(TextureContainer container, std::string fileName, std::vector<sf::IntRect> coords)
	{
		for (size_t i = 0; i < coords.size(); i++)
		{
			bool good = pushTexture(container, fileName, coords[i]);
			if (!good) { ErrorHandler::log("Unable to load [Id] = " + std::to_string(i) + "Texture from file"); }
		}
	}
	sf::Texture* getTexture(TextureContainer container, size_t index)
	{
		switch (container)
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