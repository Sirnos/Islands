#pragma once

#include "EntityBehaviorState.hpp"
#include "EntityBehaviorValues.hpp"

class EntityBehavior
{
	EntityBehaviorState currentState;
	EntityBehaviorValues values;

public:
	EntityBehavior()
		:currentState(), values()
	{}
	EntityBehavior(const EntityBehaviorState &firstState, const EntityBehaviorValues &behaviorValues)
		:currentState(firstState), values(behaviorValues)
	{}
	EntityBehavior(const EntityBehavior &other) = delete;
	~EntityBehavior() = default;


	void changeState(const EntityBehaviorState &newState)
	{
		currentState = newState;
	}
	const EntityBehaviorState &getCurrentState() const
	{
		return currentState;
	}


	void changeBehaviorValues(const EntityBehaviorValues &newValues)
	{
		values = newValues;
	}
	const EntityBehaviorValues &getBehaviorValues() const
	{
		return values;
	}


	EntityBehaviorState &getCurrentState()
	{
		return currentState;
	}
	EntityBehaviorValues &getValues()
	{
		return values;
	}
};

