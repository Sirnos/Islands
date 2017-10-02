#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

template<typename T>
sf::Rect<T> getRectFromString(const std::string &String)
{
	short varNumber = 0;
	std::string values[4];

	for (auto i : String)
	{
		if (i == ',')
		{
			if (varNumber == 3) { break; }
			varNumber++;
		}
		else
		{
			values[varNumber] += i;
		}
	}

	return sf::Rect<T>(boost::lexical_cast<T>(values[0]), boost::lexical_cast<T>(values[1]),
		boost::lexical_cast<T>(values[2]), boost::lexical_cast<T>(values[3]));
}

template<typename T>
sf::Vector2<T> getVectorFromString(const std::string &String)
{
	sf::Vector2<T> temp;
	temp.x = boost::lexical_cast<T>(String.substr(0, String.find(',')));
	temp.y = boost::lexical_cast<T>(String.substr(String.find(',') + 1, String.size() - 1));
	return temp;
}

inline bool getBoolFromString(const std::string &String)
{
	bool ret = false;
	if (String == "1")
	{
		ret = true;
	}

	return ret;
}

template<typename T>
std::pair<std::string, T> getStringAndTpairFromString(const std::string &String)
{
	std::string first{ String.substr(0, String.find(',')) };
	T second{ boost::lexical_cast<T>(String.substr(String.find(',') + 1, String.size() - 1)) };
	return std::pair<std::string, T>(first, second);
}