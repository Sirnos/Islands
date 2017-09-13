#pragma once

#include "LimitedVar.hpp"

struct EntityStats
{
	LimitedVar<float> HP;
	LimitedVar<float> MP;

	float Speed;

	EntityStats(float EntityHP, float EntityMP, float EntitySpeed)
		:HP(EntityHP),MP(EntityMP),Speed(EntitySpeed)
	{}
};