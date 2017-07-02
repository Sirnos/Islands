#pragma once

#include "ItemDef.hpp"
#include "ObjectDef.hpp"
#include <vector>

template<typename DefType>
class DefContainer
{
	std::vector<DefType> Container;

public:
	DefContainer(){}
	~DefContainer()
	{
		for (size_t i = 0; i < Container.size(); i++)
		{
			delete Container[i];
		}
		Container.clear();
	}

	size_t getSize() { return Container.size(); }

	DefType getDefinition(unsigned index)
	{
		return Container[index];
	}
	DefType getDefinition(std::string DefName)
	{
		for (size_t i = 0; i < Container.size(); i++)
		{
			if (Container[i]->getName() == DefName) { return Container[i]; }
		}
		return nullptr;
	}
	std::vector<DefType> &getContainer() { return Container; }

	void pushNewDef(DefType def)
	{
		Container.push_back(def);
	}
};

typedef DefContainer<ItemDef*> ItemDefContainer;
typedef DefContainer<ObjectDef*> ObjectDefContainer;