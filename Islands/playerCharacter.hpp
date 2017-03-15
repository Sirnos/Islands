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
		CharacterShape.setSize(sf::Vector2f(64, 96));

		
	}


};