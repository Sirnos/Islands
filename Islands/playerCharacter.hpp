#pragma once

#include "Character.hpp"

class playerCharacter :Character
{

public:
	playerCharacter()
	{
		HP = 10;
		MP = 6;
	}
	playerCharacter(sf::Texture *texture, sf::Vector2f position)
		:playerCharacter()
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(10, 10));
	}

	void move(sf::Vector2f moveVector)
	{
		CharacterShape.move(moveVector);
	}
	sf::RectangleShape *getShape()
	{
		return &CharacterShape;
	}
	void set(sf::Texture *texture, sf::Vector2f position)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(40, 60));
	}

};