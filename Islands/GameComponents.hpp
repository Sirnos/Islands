#pragma once

#include "DefContainer.hpp"
#include <memory>

class GameComponents
{
	std::shared_ptr<ObjectDefContainer> Objects;
	std::shared_ptr<ItemDefContainer> Items;
	std::shared_ptr<MonsterDefContainer> Entities;

public:
	GameComponents()
		:Objects(new ObjectDefContainer), Items(new ItemDefContainer), Entities(new MonsterDefContainer)
	{}
	~GameComponents() = default;

	std::shared_ptr<ObjectDefContainer> &getObjects()
	{
		return Objects;
	}
	std::shared_ptr<ItemDefContainer> &getItems()
	{
		return Items;
	}
	std::shared_ptr<MonsterDefContainer> &getEntities()
	{
		return Entities;
	}
};