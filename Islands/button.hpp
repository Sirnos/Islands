#pragma once

#include <SFML/Graphics.hpp>
//
#include <string>

class button
{
protected:
	sf::Text buttText;

	sf::Vector2i InteractionBoxPosition;
	sf::Vector2i InteractionBoxSize;
public:

	sf::Text getText()
	{
			return buttText;
	}
	sf::Vector2i getInterBoxSize()
	{
		return this->InteractionBoxSize;
	}
	sf::Vector2i getInterBoxPos()
	{
		return this->InteractionBoxPosition;
	}

	bool isClick(sf::Vector2i mousePos, bool isClicked)
	{
		sf::Vector2i BoxPos = this->InteractionBoxPosition;
		sf::Vector2i BoxSize = this->InteractionBoxSize;

		if (mousePos.x >= BoxPos.x && mousePos.x <= BoxPos.x + BoxSize.x
			&& mousePos.y >= BoxPos.y && mousePos.y <= BoxPos.y + BoxSize.y && isClicked == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void setLogicBoxSize(sf::Vector2i newSize)
	{
		this->InteractionBoxSize = newSize;
	}
	void setLogicBoxPos(sf::Vector2i newPosition)
	{
		this->InteractionBoxPosition = newPosition;
	}
	void setButtonPosition(sf::Vector2f newPos)
	{
		this->buttText.setPosition(newPos);
	}
	void setTextOutlineColor(sf::Color col)
	{
		this->buttText.setOutlineColor(col);
	}
	void setTextFillColor(sf::Color col)
	{
		this->buttText.setFillColor(col);
	}
	void setTextStyle(sf::Text::Style style)
	{
		this->buttText.setStyle(style);
	}
	void setString(std::string str)
	{
		this->buttText.setString(str);
	}
	void setCharacterSize(unsigned int Size)
	{
		this->buttText.setCharacterSize(Size);
	}
	void setFont(sf::Font &fon)
	{
		this->buttText.setFont(fon);
	}
	void setColor(sf::Color col)
	{
		this->buttText.setFillColor(col);
		this->buttText.setOutlineColor(col);
	}

	void create(sf::Vector2f pos, sf::Vector2i Lpos, sf::Vector2i Lsize, std::string str, sf::Font &fon)
	{
		this->buttText.setPosition(pos);
		this->buttText.setString(str);
		this->buttText.setFont(fon);
		this->InteractionBoxPosition = Lpos;
		this->InteractionBoxSize = Lsize;
	}


	button(){}
	~button(){}
};