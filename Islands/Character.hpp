#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

class Character
{
protected:

	sf::RectangleShape CharacterShape;
	float HP;
	float MP;

	virtual void attack() {}
	virtual void idle() {}
public:
	Character()
	{

	}
	~Character()
	{

	}

	virtual void move(){}
	virtual void behavior(){}

	float getHP() { return HP; }
	float getMP() { return MP; }


};