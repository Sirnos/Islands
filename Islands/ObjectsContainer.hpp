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
	ObjectContainer(unsigned RangeForElements = 512,Elements &otherContainer = Elements())
	{
		Container.resize(RangeForElements);
		for (auto & i : Container)
		{
			i.resize(RangeForElements,0);
		}
	}
	~ObjectContainer() { Container.clear(); }

	void pushNewObject(unsigned ObjectID, sf::Vector2u position) { Container[position.x][position.y] = ObjectID; }
	const Elements &getContainerElements() { return Container; }
	unsigned getElementID(sf::Vector2u position) { return Container[position.x][position.y]; }
};