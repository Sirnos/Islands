#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "EntitySide.hpp"
#include "ErrorHandler.hpp"

enum class TextureContainer : unsigned
{
	TerrainTextures,
	EntitiesTextures,
	ObjectTextures,
	ItemsTextures,
};

inline std::string TextureContainerToString(TextureContainer type)
{
	std::string ret("TextureContainer::");
	switch (type)
	{
	case TextureContainer::TerrainTextures:
		ret += "Terrain";
		break;
	case TextureContainer::EntitiesTextures:
		ret += "Entities";
		break;
	case TextureContainer::ObjectTextures:
		ret += "Objects";
		break;
	case TextureContainer::ItemsTextures:
		ret += "Items";
		break;
	default:
		ret += "!IsUndefided";
		return ret;
		break;
	}
	ret += "Textures";
	return ret;
}

class Media
{
	std::vector<sf::Texture> TerrainTexture;
	std::vector<sf::Texture> ObjectsTexture;
	std::vector<sf::Texture> ItemsTexture;

	std::vector<std::pair<sf::Texture, sf::Texture>> EntitiesTexture;
public:
	Media()
		:TerrainTexture(1), ObjectsTexture(1), ItemsTexture(1), EntitiesTexture(1)
	{
		pushEntitiesTexture(std::string("Data/Entities.png"), sf::Vector2i(40, 80), sf::Vector2i(0, 0));
	}
	~Media() = default;

	bool pushTexture(const TextureContainer &container, const std::string &fileName, const sf::IntRect &textCord)
	{
		if (container == TextureContainer::EntitiesTextures)
		{
			ErrorHandler::log(std::string("Use pushEntitiesTexture() instead"));
			return false;
		}
		sf::Texture texture;
		bool isSuccessful = false;
		isSuccessful = texture.loadFromFile(fileName, textCord);

		switch (container)
		{
		case TextureContainer::TerrainTextures:
			TerrainTexture.push_back(texture);
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
		return isSuccessful;
	}
	bool pushEntitiesTexture(const std::string& fileName, const sf::Vector2i &EntitySize, const sf::Vector2i &entityTextureBeginPosition)
	{
		sf::Texture rightSide;
		sf::Texture leftSide;

		bool isRightSideSuccessful = rightSide.loadFromFile(fileName, sf::IntRect(entityTextureBeginPosition, EntitySize));
		bool isLeftSideSuccessful = leftSide.loadFromFile(fileName, sf::IntRect(entityTextureBeginPosition + sf::Vector2i(EntitySize.x, 0), EntitySize));

		EntitiesTexture.push_back(std::pair<sf::Texture, sf::Texture>(leftSide, rightSide));
		return isLeftSideSuccessful && isRightSideSuccessful;
	}

	void pushTextures(const TextureContainer &container, const std::string &fileName, const std::vector<sf::IntRect> &coords)
	{
		bool good;
		for (size_t i = 0; i < coords.size(); i++)
		{
			good = false;
			if (container != TextureContainer::EntitiesTextures)
			{
				good = pushTexture(container, fileName, coords[i]);
			}
			else
			{
				good = pushEntitiesTexture(fileName, sf::Vector2i(coords[i].width, coords[i].height), sf::Vector2i(coords[i].left, coords[i].top));
			}
			if (!good)
			{
				ErrorHandler::log("Unable to load [Id] = " + std::to_string(i) + " [Type] " + TextureContainerToString(container));
			}

		}
	}
	sf::Texture* getTexture(const TextureContainer &container, size_t index, EntitySide EntitySideTexture = EntitySide::Right)
	{
		switch (container)
		{
		case TextureContainer::TerrainTextures:
			return &TerrainTexture[index];
			break;
		case TextureContainer::EntitiesTextures:
			if (EntitySideTexture == EntitySide::Left)
			{
				return &EntitiesTexture[index].second;
			}
			return &EntitiesTexture[index].first;
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

	size_t getSizeOfContainer(const TextureContainer &typeOfContainer) const
	{
		switch (typeOfContainer)
		{
		case TextureContainer::TerrainTextures:
			return TerrainTexture.size();
			break;
		case TextureContainer::EntitiesTextures:
			return EntitiesTexture.size();
			break;
		case TextureContainer::ObjectTextures:
			return ObjectsTexture.size();
			break;
		case TextureContainer::ItemsTextures:
			return ItemsTexture.size();
			break;
		}
	}
};