#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

typedef std::vector<std::vector<unsigned>> Elements;

//Container for simple Objects like tress,
//non for chest,crafting table etc. objects
class ObjectContainer
{
	Elements Container;
public:
	ObjectContainer(){}
	~ObjectContainer() { Container.clear(); }

	void generate(unsigned Size)
	{
		Container.resize(Size, std::vector<unsigned>(Size, 0));
	}

	void pushNewObject(unsigned ObjectID, sf::Vector2u position) { Container[position.x][position.y] = ObjectID; }
	const Elements &getContainerElements() { return Container; }
	unsigned getElementID(sf::Vector2u position) { return Container[position.x][position.y]; }
};