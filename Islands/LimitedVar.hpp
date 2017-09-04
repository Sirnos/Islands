#pragma once

template<typename T>
class LimitedVar
{
	T Var;
	const T Limit;

	void _round()
	{
		if (Var > Limit)
		{
			Var = Limit;
		}
	}
public:
	LimitedVar(T Max)
		:Limit(Max), Var(Max)
	{}
	LimitedVar(const LimitedVar<T> & other)
		:Var(other.Var), Limit(other.Limit)
	{}
	~LimitedVar() = default;

	LimitedVar<T> &operator+=(const LimitedVar<T> &Other)
	{
		Var += Other.Var;
		_round();
		return *this;
	}
	LimitedVar<T> &operator+=(const T& Other)
	{
		Var += Other;
		_round();
		return *this;
	}
	LimitedVar<T> &operator-=(const LimitedVar<T> &Other)
	{
		Var -= *Other.Var;
		_round();
		return *this;
	}
	LimitedVar<T> &operator-=(const T& Other)
	{
		Var -= Other;
		_round();
		return *this;
	}
	LimitedVar<T> &operator*=(const LimitedVar<T> &Other)
	{
		Var *= Other.Var;
		_round();
		return *this;
	}
	LimitedVar<T> &operator*=(const T& Other)
	{
		Var *= Other;
		_round();
		return *this;
	}
	LimitedVar<T> &operator/=(const LimitedVar<T> &Other)
	{
		Var /= Other.Var;
		_round();
		return *this;
	}
	LimitedVar<T> &operator/=(const T& Other)
	{
		Var /= Other;
		_round();
		return *this;
	}
	LimitedVar<T> &operator=(const T& Other)
	{
		Var = Other;
		_round();
		return *this;
	}

	T getVar() { return Var; }
	T getLimit() { return Limit; }
};
