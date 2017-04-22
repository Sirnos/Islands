#pragma once

#include <SFML/Graphics/Text.hpp>
#include <string>

class button
{
protected:
	sf::Text buttText;
	sf::Vector2i InteractionBoxPosition;
	sf::Vector2i InteractionBoxSize;
public:

	sf::Text getText() { return buttText; }
	sf::Vector2i getInterBoxSize() { return InteractionBoxSize; }
	sf::Vector2i getInterBoxPos() { return InteractionBoxPosition; }

	bool isClick(sf::Vector2i mousePos, bool isClicked)
	{
		sf::Vector2i BoxPos = InteractionBoxPosition;
		sf::Vector2i BoxSize = InteractionBoxSize;

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

	void setLogicBoxSize(sf::Vector2i newSize) { InteractionBoxSize = newSize; }
	void setLogicBoxPos(sf::Vector2i newPosition) { InteractionBoxPosition = newPosition; }
	void setButtonPosition(sf::Vector2f newPos) { buttText.setPosition(newPos); }
	void setTextOutlineColor(sf::Color col) { buttText.setOutlineColor(col); }
	void setTextFillColor(sf::Color col) { buttText.setFillColor(col); }
	void setTextStyle(sf::Text::Style style) { buttText.setStyle(style); }
	void setString(std::string str) { buttText.setString(str); }
	void setCharacterSize(unsigned Size) { buttText.setCharacterSize(Size); }
	void setFont(sf::Font &fon) { buttText.setFont(fon); }

	void setColor(sf::Color col)
	{
		buttText.setFillColor(col);
		buttText.setOutlineColor(col);
	}

	void create(sf::Vector2f pos, sf::Vector2i Lpos, sf::Vector2i Lsize, std::string str, sf::Font &fon)
	{
		buttText.setPosition(pos);
		buttText.setString(str);
		buttText.setFont(fon);
		InteractionBoxPosition = Lpos;
		InteractionBoxSize = Lsize;
	}

	button(){}
	~button(){}
};