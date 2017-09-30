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

	std::shared_ptr<World> MonsterWorld;


	void _monsterFollow(const Entity * toFollow, size_t Monster)
	{
		sf::Vector2f followedEntityPos = toFollow->getCharacterCenterPosition();
		sf::Vector2f followerEntityPos = ManagementMonsters[Monster].getCharacterCenterPosition();


		float distance = std::sqrt(std::pow(followerEntityPos.x - followedEntityPos.x, 2)) + std::sqrt(std::pow(followerEntityPos.y - followedEntityPos.y, 2));
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


	void spawnMonster(const MonsterEntityDef &MonsterDef, const size_t MonsterId, const sf::Vector2f &SpawnPosition)
	{
		ManagementMonsters.push_back(Monster(sf::RectangleShape(MonsterDef.getSize()), MonsterDef.getStats(), MonsterId));
		ManagementMonsters.back().setPosition(SpawnPosition);
	}
	void spawnMonsters(const std::vector<MonsterEntityDef> &MonstersDef, const std::vector<size_t> &IDs, const std::vector<sf::Vector2f> &SpawnPositions)
	{
		size_t i = 0;
		while (i < MonstersDef.size() && i < IDs.size() && i < SpawnPositions.size())
		{
			spawnMonster(MonstersDef[i], IDs[i], SpawnPositions[i]);
			i++;
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
	const std::vector<Monster> &getMonsters() const
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


	void assingMonsterWorld(std::shared_ptr<World> &ptrToWorld)
	{
		MonsterWorld = ptrToWorld;
	}


	void manageMonster(size_t Monster)
	{
		_monsterFollow(ObservedEntities.front(), Monster);
	}
};