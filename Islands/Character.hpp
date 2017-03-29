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
	Character(){}
	~Character(){}

	virtual void move(){}
	virtual void behavior(){}

	float getHP() { return HP; }
	float getMP() { return MP; }

	sf::Vector2f getPosition()
	{
		return CharacterShape.getPosition();
	}
	sf::Vector2f getCharacterCenterPosition()
	{
		sf::Vector2f position = CharacterShape.getPosition();
		position.x += CharacterShape.getSize().x / 2;
		position.y += CharacterShape.getSize().y / 2;

		return position;
	}
	sf::Vector2f getSize()
	{
		return CharacterShape.getSize();
	}
	void setPosition(sf::Vector2f newPosition)
	{
		CharacterShape.setPosition(newPosition);
	}

};