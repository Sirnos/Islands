#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "EntityStats.hpp"

class Entity
{
protected:
	sf::RectangleShape Body;

	virtual void attack() {}
	virtual void idle() {}
public:
	EntityStats Stats;

	Entity(sf::RectangleShape EntityBody,float HP,float MP,float Speed) 
		:Stats(HP,MP,Speed),
		Body(EntityBody)
	{}

	virtual void move(){}
	virtual void operator()(){}

	sf::Vector2f getCharacterCenterPosition()
	{
		sf::Vector2f position = Body.getPosition();
		position.x += Body.getSize().x / 2;
		position.y += Body.getSize().y / 2;

		return position;
	}
	void setPosition(sf::Vector2f newPosition)
	{
		Body.setPosition(newPosition);
	}

	const sf::RectangleShape &getBody() { return Body; }

	void pushTexture(sf::Texture *Texture) { Body.setTexture(Texture); }
};