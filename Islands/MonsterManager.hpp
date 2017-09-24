#pragma once

#include <vector>
#include <cmath>
#include "Entity.hpp"
#include "EntityDef.hpp"

class MonsterManager
{
	const float DistanceToFollow = 256.0f;

	std::vector<Monster> ManagementMonsters;
	std::vector<Entity*> ObservedEntities;


	void _monsterFollow(const Entity * toFollow, size_t Monster)
	{
		sf::Vector2f followedEntityPos = toFollow->getCharacterCenterPosition();
		sf::Vector2f followerEntityPos = ManagementMonsters[Monster].getCharacterCenterPosition();

		float distance = sqrt(pow(followerEntityPos.x - followedEntityPos.x, 2)) + sqrt(pow(followerEntityPos.y - followedEntityPos.y, 2));
		if (distance >= DistanceToFollow)
		{
			sf::Vector2f DistanceVector = followedEntityPos - followerEntityPos;
			sf::Vector2f moveVector;
			float MonsterSpeed = ManagementMonsters[Monster].Stats.Speed;

			if (DistanceVector.x > MonsterSpeed)
			{
				moveVector.x = MonsterSpeed;
			}
			else if(DistanceVector.x < -MonsterSpeed)
			{
				moveVector.x = -MonsterSpeed;
			}
			if (DistanceVector.y > MonsterSpeed)
			{
				moveVector.y = MonsterSpeed;
			}
			else if(DistanceVector.y < -MonsterSpeed)
			{
				moveVector.y = -MonsterSpeed;
			}
			ManagementMonsters[Monster].move(moveVector);
		}
	}
public:
	MonsterManager() = default;
	MonsterManager(const MonsterManager& other) = delete;
	MonsterManager(const std::vector<Monster> &SavedMonsters)
		:ManagementMonsters(SavedMonsters)
	{}
	~MonsterManager() = default;


	void addMonster(const Monster& toAdd)
	{
		ManagementMonsters.push_back(toAdd);
	}
	void addMonster(const MonsterEntityDef& toAdd, unsigned MonsterId)
	{
		ManagementMonsters.push_back(Monster(sf::RectangleShape(toAdd.getSize()), toAdd.getStats(), MonsterId));
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
	void removeAllMonsters()
	{
		ManagementMonsters.clear();
	}
	Monster & getMonster(size_t index)
	{
		return ManagementMonsters[index];
	}


	const std::vector<Monster> &getManagementMonsters() const
	{
		return ManagementMonsters;
	}


	void addEntityToObserved(Entity *ptrToEntity)
	{
		ObservedEntities.push_back(ptrToEntity);
	}
	void removeEntityFromObserved(size_t index)
	{
		ObservedEntities.erase(ObservedEntities.begin() + index);
	}
	void removeAllObserved()
	{
		ObservedEntities.clear();
	}


	void manageMonster(size_t Monster)
	{
		_monsterFollow(ObservedEntities.front(), Monster);
	}
};