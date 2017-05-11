#pragma once
#include <vector>
#include "ItemDef.hpp"
#include "ObjectDef.hpp"

class GameComponentUnloader
{
public:
	void clearItems(std::vector<ItemDef*> &ItemsContainer)
	{
		for (auto & i : ItemsContainer)
		{
			delete i;
		}
		ItemsContainer.clear();
	}
};