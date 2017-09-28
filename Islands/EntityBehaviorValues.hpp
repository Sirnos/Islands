#pragma once

enum class EntityAttackType
{
	Melee,
	Distance
};

enum class EntityAggressive
{
	Passive,
	Fearful,
	Violent
};


struct EntityBehaviorValues
{
	unsigned maxGroupSize;
	bool canLiveInGroup;

	EntityAggressive aggresion;
	EntityAttackType attackPrefer;


	EntityBehaviorValues()
		:maxGroupSize(0), canLiveInGroup(false), attackPrefer(EntityAttackType::Melee), aggresion(EntityAggressive::Passive)
	{}
	EntityBehaviorValues(unsigned groupSize, bool canGroup, EntityAttackType attckPref, EntityAggressive aggressive)
		:maxGroupSize(groupSize), canLiveInGroup(canGroup), attackPrefer(attckPref), aggresion(aggressive)
	{}
	EntityBehaviorValues(const EntityBehaviorValues &other)
		:maxGroupSize(other.maxGroupSize), canLiveInGroup(other.canLiveInGroup), attackPrefer(other.attackPrefer), aggresion(other.aggresion)
	{}
	~EntityBehaviorValues() = default;

	EntityBehaviorValues &operator=(const EntityBehaviorValues &other)
	{
		maxGroupSize = other.maxGroupSize;
		canLiveInGroup = other.canLiveInGroup;
		aggresion = other.aggresion;
		attackPrefer = other.attackPrefer;

		return *this;
	}
};