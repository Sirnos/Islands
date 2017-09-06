#pragma once

#include <random>
#include <type_traits>

class RandomNumberGenerator
{
protected:
	std::mt19937 generator;

	RandomNumberGenerator()
	{
		std::random_device dev;
		generator.seed(dev());
	}
	RandomNumberGenerator(const RandomNumberGenerator &other) = delete;
	~RandomNumberGenerator() = default;
};

template<typename T,typename = typename std::enable_if<std::is_integral<T>::value,T>::type>
class IntegerGenerator : public RandomNumberGenerator
{
public:
	IntegerGenerator() = default;
	IntegerGenerator(const IntegerGenerator &other) = delete;
	~IntegerGenerator() = default;

	T get(T min, T max)
	{
		std::uniform_int_distribution<T> distributor{ min,max };
		return distributor(generator);
	}
};

template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
class FloatGenerator : public RandomNumberGenerator
{
public:
	FloatGenerator() = default;
	FloatGenerator(const FloatGenerator &other) = delete;
	~FloatGenerator() = default;

	T get(T min, T max)
	{
		std::uniform_real_distribution<T> distributor{ min,max };
		return distributor(generator);
	}
};