#pragma once

#include <SFML/System/Vector2.hpp>

class CollisionDetect
{
public:
	static bool isPointInRectangle(sf::Vector2f point, sf::Vector2f rectPos, sf::Vector2f rectSize)
	{
		if (point.x > rectPos.x && point.x < rectPos.x + rectSize.x &&
			point.y > rectPos.y && point.y < rectPos.y + rectSize.y)
		{
			return true;
		}

		return false;
	}
};