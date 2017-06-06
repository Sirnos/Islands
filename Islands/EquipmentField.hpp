#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "ItemField.hpp"

const float DefaultEqFieldSize = 64;
const sf::Color DefaultEqFieldColor = sf::Color(120, 250, 120, 250);

struct EquipmentField
{
	ItemField item;

	sf::RectangleShape FieldRect;
	sf::RectangleShape TextureRect;

	void create(sf::Vector2f atPosition, float fieldSize, sf::Color fieldColor)
	{
		FieldRect.setFillColor(fieldColor);
		setPosition(atPosition);
		setSize(fieldSize);
	}
	void setPosition(sf::Vector2f newPosition)
	{
		FieldRect.setPosition(newPosition);
		TextureRect.setPosition(newPosition);
	}
	void setSize(float fieldSize)
	{
		FieldRect.setSize(sf::Vector2f(fieldSize, fieldSize));
		TextureRect.setSize(sf::Vector2f(fieldSize, fieldSize));
	}

	void assingItem(ItemField other)
	{
		item.assingFromOther(other);
	}
	void assingTexture(sf::Texture *texture)
	{
		TextureRect.setTexture(texture);
	}
	void clearField()
	{
		item.clear();
		TextureRect.setTexture(nullptr);
	}
};