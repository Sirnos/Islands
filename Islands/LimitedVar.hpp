#pragma once

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value,T>::type>
class LimitedVar
{
	T Var;
	T Limit;

	void _round()
	{
		if (Var > Limit)
		{
			Var = Limit;
		}
	}
public:
	LimitedVar()
		:Var(0), Limit(std::numeric_limits<T>::max() - 1)
	{}
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


	LimitedVar<T> &operator=(const LimitedVar<T> &other)
	{
		Var = other.Var;
		Limit = other.Limit;

		return *this;
	}

	T getVar() const { return Var; }
	T getLimit() const { return Limit; }

	void setVar(T newVar)
	{
		Var = newVar;
		_round();
	}
	void setLimit(T newLimit)
	{
		Limit = newLimit;
		_round();
	}
};
