#pragma once

#include <vector>
#include "ItemDefFactory.hpp"

class ItemDefContainer
{
	std::vector<ItemDef*> Container;

public:
	ItemDef* getItemDef(size_t index)
	{
		return Container[index];
	}
	void pushNewItemDef(ItemDef* Item)
	{
		Container.push_back(Item);
	}

	ItemDefContainer() { Container.push_back(nullptr); }
	~ItemDefContainer()
	{
		for (auto & i : Container) { delete i; }
		Container.clear();
	}
};