#pragma once

#include <vector>
#include "Entity.hpp"

class MonsterManager
{
	std::vector<Monster> ManagementMonsters;
public:
	MonsterManager() = default;
	MonsterManager(const MonsterManager& other) = delete;
	MonsterManager(const std::vector<Monster> &SavedMonsters)
	{
		ManagementMonsters = SavedMonsters;
	}
	~MonsterManager() = default;


	void addMonster(const Monster& toAdd)
	{
		ManagementMonsters.push_back(toAdd);
	}
	void addMonsters(const std::vector<Monster> &Monsters)
	{
		for (const auto m : Monsters)
		{
			ManagementMonsters.push_back(m);
		}
	}
	void removeMonster(size_t index)
	{
		ManagementMonsters.erase(ManagementMonsters.begin() + index);
	}
	Monster & getMonster(size_t index)
	{
		return ManagementMonsters[index];
	}


	const std::vector<Monster> &getManagementMonsters()
	{
		return ManagementMonsters;
	}
};