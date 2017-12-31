#pragma once

#include "LimitedVar.hpp"

struct EntityStats
{
	LimitedVar<float> HP;
	LimitedVar<float> MP;

	float Speed;

	EntityStats() = delete;
	EntityStats(float EntityHP, float EntityMP, float EntitySpeed)
		:HP(EntityHP), MP(EntityMP), Speed(EntitySpeed)
	{}
	EntityStats(const EntityStats &other)
		:HP(other.HP), MP(other.MP), Speed(other.Speed)
	{}
	EntityStats(float EntityHPMax, float EntityMPMax, float Speed, float EntityHP, float EntityMP)
		:HP(EntityHPMax), MP(EntityMPMax), Speed(Speed)
	{
		HP.setVar(EntityHP);
		MP.setVar(EntityMP);
	}
	~EntityStats() = default;


	EntityStats &operator=(const EntityStats &other)
	{
		HP = other.HP;
		MP = other.MP;
		Speed = other.Speed;

		return *this;
	}
};