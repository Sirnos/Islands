#pragma once

#include <vector>
#include "ItemDef.hpp"

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
	std::vector<ItemDef*>& getContainer() { return Container; }
};