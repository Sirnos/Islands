#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>


typedef std::pair<std::string, unsigned> Yield;

class ObjectDef
{
	std::string ObjName;
	sf::Vector2f ObjSize;
	sf::IntRect ObjTextureCord;
	sf::FloatRect ObjCollisionBox;
	bool ObjDestructible;
	Yield ObjYield;
public:
	ObjectDef(std::string Name, sf::Vector2f Size, sf::IntRect TextureCord, sf::FloatRect CollisionBox,
		Yield yield, bool Destructible)
		:ObjDestructible(Destructible),
		ObjName(Name),
		ObjSize(Size),
		ObjTextureCord(TextureCord),
		ObjCollisionBox(CollisionBox),
		ObjYield(yield)
	{}
	ObjectDef()
		:ObjDestructible(false),
		ObjName(""),
		ObjSize(0,0),
		ObjTextureCord(0,0,0,0),
		ObjCollisionBox(0,0,0,0),
		ObjYield(Yield("",0))
	{}

	std::string getName() { return ObjName; }
	sf::Vector2f getSize() { return ObjSize; }
	sf::IntRect getTextureCord() { return ObjTextureCord; }
	sf::FloatRect getCollisionBox() { return ObjCollisionBox; }
	bool getDestructible() { return ObjDestructible; }
	Yield getYield() { return ObjYield; }
};