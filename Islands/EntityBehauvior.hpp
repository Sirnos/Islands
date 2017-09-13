#pragma once

enum class AttackTypePrefer
{
	Melee,
	Distance
};

struct BehauviorVariables
{
	unsigned agressive;

	unsigned maxGroupSize;
	bool canLiveInGroup;

	AttackTypePrefer attackPrefer;


	BehauviorVariables() = delete;
	BehauviorVariables(unsigned aggres, unsigned groupSize, bool canGroup, AttackTypePrefer attckPref)
		:agressive(aggres), maxGroupSize(groupSize), canLiveInGroup(canGroup), attackPrefer(attckPref)
	{}
	BehauviorVariables(const BehauviorVariables &other) = default;
	~BehauviorVariables() = default;
};