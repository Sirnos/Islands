#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "EntityStats.hpp"
#include "EntitySide.hpp"

class Entity
{
protected:
	sf::RectangleShape Body;

	EntitySide ActualSide;

	virtual void attack() {}
	virtual void idle() {}
public:
	EntityStats Stats;

	Entity(sf::RectangleShape EBody, float EHP, float EMP, float ESpeed)
		:Stats(EHP, EMP, ESpeed), Body(EBody)
	{}
	Entity(sf::RectangleShape EBody, EntityStats EStats)
		:Body(EBody), Stats(EStats)
	{}

	void move(sf::Vector2f vectr) 
	{ 
		if (vectr.x < 0.0f)
		{
			ActualSide = EntitySide::Left;
		}
		else if(vectr.x > 0.0f)
		{
			ActualSide = EntitySide::Right;
		}
		Body.move(vectr);
	}
	virtual void operator()(){}

	sf::Vector2f getCharacterCenterPosition() const
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

	const sf::RectangleShape &getBody() const { return Body; }
	EntitySide getActualEntitySide() const { return ActualSide; }

	void pushTexture(sf::Texture *Texture) { Body.setTexture(Texture); }
};

class Monster : public Entity
{
	unsigned Id;
public:
	Monster(sf::RectangleShape MBody, float MHP, float MMP, float MSpeed, unsigned MId)
		:Entity(MBody, MHP, MMP, MSpeed), Id(MId)
	{}
	Monster(sf::RectangleShape MBody, const EntityStats MStats, unsigned MId)
		:Entity(MBody, MStats), Id(MId)
	{}
	Monster(const Monster &other)
		:Entity(other.Body, other.Stats), Id(other.Id)
	{}

	unsigned getId() const { return Id; }
};