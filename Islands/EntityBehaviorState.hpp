#pragma once

#include <SFML/System/Clock.hpp>

enum class EntityBehaviorStateType : unsigned
{
	Idle,
	Search,
	Follow,
	Attack,
	Reproduce
};

struct EntityBehaviorState
{
	EntityBehaviorStateType Type;

	sf::Time StateBeginTime;
	sf::Time StateTime;


	EntityBehaviorState()
		:Type(EntityBehaviorStateType::Idle)
	{}
	EntityBehaviorState(const EntityBehaviorStateType &beginType, const sf::Time &BeginTime, const sf::Time &ForTime)
		:Type(beginType), StateBeginTime(BeginTime), StateTime(ForTime)
	{}
	EntityBehaviorState(const EntityBehaviorState &other)
		:Type(other.Type), StateBeginTime(other.StateBeginTime), StateTime(other.StateTime)
	{}
	~EntityBehaviorState() = default;


	EntityBehaviorState & operator=(const EntityBehaviorState &other)
	{
		Type = other.Type;
		StateBeginTime = other.StateBeginTime;
		StateTime = other.StateTime;

		return *this;
	}
};