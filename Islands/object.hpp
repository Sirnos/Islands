#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

class object
{
private:
	sf::RectangleShape objectShape;

public:
	void setPosition(sf::Vector2f pos)
	{
		objectShape.setPosition(pos);
	}
	void setSize(sf::Vector2f size)
	{
		objectShape.setSize(size);
	}

	void setTexture(sf::Texture *txtr)
	{
		objectShape.setTexture(txtr);
	}

	sf::Vector2f getPosition()
	{
		return objectShape.getPosition();
	}
	sf::Vector2f getSize()
	{
		return objectShape.getSize();
	}
	sf::RectangleShape *getShape()
	{
		return &objectShape;
	}
};