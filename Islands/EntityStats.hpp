#pragma once

#include "LimitedVar.hpp"

struct EntityStats
{
	LimitedVar<float> HP;
	LimitedVar<float> MP;

	float Speed;

	EntityStats() = delete;
	EntityStats(float EntityHP, float EntityMP, float EntitySpeed)
		:HP(EntityHP),MP(EntityMP),Speed(EntitySpeed)
	{}
	EntityStats(const EntityStats &other)
		:HP(other.HP),MP(other.MP),Speed(other.Speed)
	{}
	~EntityStats() = default;
};