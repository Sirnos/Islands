#pragma once

template<typename T>
class LimitedVar
{
	T Var;
	const T Limit;

	void Round()
	{
		Var = Limit;
	}
public:
	LimitedVar(T Max)
		:Limit(Max),
		Var(Limit)
	{}

	LimitedVar<T> &operator+=(const LimitedVar<T> &Other)
	{
		Var += Other.Var;
		if (Var > Limit) { Round(); }
		return *this;
	}
	LimitedVar<T> &operator+=(const T& Other)
	{
		Var += Other;
		if (Var > Limit) { Round(); }
		return *this;
	}
	LimitedVar<T> &operator-=(const LimitedVar<T> &Other)
	{
		Var -= *Other.Var;
		return this;
	}
	LimitedVar<T> &operator-=(const T& Other)
	{
		Var -= Other;
		return *this;
	}
	LimitedVar<T> &operator*=(const LimitedVar<T> &Other)
	{
		Var *= Other.Var;
		if (Var > Limit) { Round(); }
		return *this;
	}
	LimitedVar<T> &operator*=(const T& Other)
	{
		Var *= Other;
		if (Var > Limit) { Round(); }
	}
	LimitedVar<T> &operator/=(const LimitedVar<T> &Other)
	{
		Var /= Other.Var;
		return *this;
	}
	LimitedVar<T> &operator/=(const T& Other)
	{
		Var /= Other;
		return *this;
	}

	T getVar() { return Var; }
	T getLimit() { return Limit; }
};

struct EntityStats
{
	LimitedVar<float> HP;
	LimitedVar<float> MP;

	float Speed;

	EntityStats(float EntityHP, float EntityMP, float EntitySpeed)
		:HP(EntityHP),
		MP(EntityMP)
	{
		Speed = EntitySpeed;
	}
};