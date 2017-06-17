#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "ItemField.hpp"

const float DefaultEqFieldSize = 64;
const sf::Color DefaultEqFieldColor = sf::Color(120, 250, 120, 250);
const sf::Color EqFieldColorWhenIsSelected = sf::Color(250, 60, 80, 250);

struct EquipmentField
{
	sf::RectangleShape FieldRect;
	sf::RectangleShape TextureRect;

	bool ishover;

	void create(sf::Vector2f atPosition, float fieldSize, sf::Color fieldColor)
	{
		FieldRect.setFillColor(fieldColor);
		setPosition(atPosition);
		setSize(fieldSize);
		ishover = false;
	}

	void setSize(float fieldSize)
	{
		FieldRect.setSize(sf::Vector2f(fieldSize, fieldSize));
		TextureRect.setSize(sf::Vector2f(fieldSize, fieldSize));
	}
	void setTexture(sf::Texture *texture)
	{
		TextureRect.setTexture(texture);
	}

	void setPosition(sf::Vector2f newPosition)
	{
		FieldRect.setPosition(newPosition);
		TextureRect.setPosition(newPosition);
	}
	sf::Vector2f getPosition() { return FieldRect.getPosition(); }
};