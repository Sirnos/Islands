#pragma once

#include <SFML/Graphics.hpp>

class Object
{
	std::string ID;
	sf::Vector2f Size;
	sf::IntRect TextureCord;
	sf::FloatRect CollisionBox;
	bool Destructible;

	//1.Name of item 2.amount
	std::pair<std::string, unsigned> Yield;
public:
	Object()
	{
		Size = { 0,0 };
		CollisionBox = { 0.0f,0.0f,0.0f,0.0f };
		Destructible = { false };
		Yield = { "NULL",0 };
		TextureCord = { 0,0,0,0 };
	}
	Object(std::string name,sf::Vector2f begSize, sf::IntRect begTextureCord,
		sf::FloatRect begCollisionBox, bool begDestructible, std::string YieldType,unsigned YieldAmount )
	{
		ID = name;
		Size = begSize;
		TextureCord = begTextureCord;
		CollisionBox = begCollisionBox;
		Destructible = begDestructible;
		Yield.first = YieldType;
		Yield.second = YieldAmount;
	}

	bool operator==(const Object &second)
	{
		if (ID == second.ID)
		{
			if (Size == second.Size)
			{
				if (TextureCord == second.TextureCord)
				{
					if (CollisionBox == second.CollisionBox)
					{
						if (Destructible == second.Destructible)
						{
							if (Yield.first == second.Yield.first)
							{
								if (Yield.second == second.Yield.second)
								{
									true;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	std::string getID()
	{
		return ID;
	}
	sf::Vector2f getSize()
	{
		return Size;
	}
	sf::IntRect getTextureCord()
	{
		return TextureCord;
	}
	sf::FloatRect getCollisionBox()
	{
		return CollisionBox;
	}
	bool getDestructible()
	{
		return Destructible;
	}
	std::pair<std::string, unsigned> getYield()
	{
		return Yield;
	}

	/*
	void setID(std::string newValue)
	{
		ID = newValue;
	}
	void setSize(sf::Vector2f newValue)
	{
		Size = newValue;
	}
	void setTextureCord(sf::IntRect newValue)
	{
		TextureCord = newValue;
	}
	void setCollisionBox(sf::FloatRect newValue)
	{
		CollisionBox = newValue;
	}
	void setDestructible(bool newValue)
	{
		Destructible = newValue;
	}
	void setYield(const std::pair<std::string, unsigned> &newValue)
	{
		Yield = newValue;
	}
	*/
};