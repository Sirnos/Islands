#pragma once

#include <SFML/Graphics.hpp>

class textbox
{
	size_t MAX_TEXT_SIZE = 30;

	bool IsEnable = false;
	bool IsConstant = false;
	std::string String;
	sf::Text TXBOX_text;

public:

	void operator()(sf::Event &event)
	{
		if (IsEnable == false || IsConstant == true)
		{
			return;
		}
		else
		{
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8)
				{
					if (String.size() > 0 == true)
					{
						String.pop_back();
						TXBOX_text.setString(String);
						return;
					}
				}

				if (String.size() >= MAX_TEXT_SIZE)
				{
					return;
				}

				if (event.text.unicode < 128)
				{
					String += static_cast<char>(event.text.unicode);
					TXBOX_text.setString(String);
				}
			}
		}
	}

	void create(sf::Font &font, bool isConstant, std::string strForTextBox, size_t MAXSIZE)
	{
		TXBOX_text.setFont(font);
		IsConstant = isConstant;
		TXBOX_text.setString(strForTextBox);
		MAX_TEXT_SIZE = MAXSIZE;
	}
	void changeVars(sf::Color colorForText, unsigned int CharSize, sf::Uint32 style)
	{
		TXBOX_text.setFillColor(colorForText);
		TXBOX_text.setOutlineColor(colorForText);
		TXBOX_text.setCharacterSize(CharSize);
		TXBOX_text.setStyle(style);
	}
	bool isClick(sf::Vector2i mousePos,bool isMouseClick)
	{
		sf::Vector2i textBoxPos = static_cast<sf::Vector2i>(TXBOX_text.getPosition());

		int sizeX = 100;
		if (MAX_TEXT_SIZE > 5)
		{
			sizeX = MAX_TEXT_SIZE * 10;
		}
		if ((mousePos.x >= textBoxPos.x && mousePos.x <= mousePos.x + sizeX &&
			mousePos.y >= textBoxPos.y && mousePos.y <= mousePos.y + 50) == true
			&& isMouseClick == true)
		{
			return true;
		}

		return false;
	}

	std::string getString()
	{
		return TXBOX_text.getString();
	}
	void setString(std::string str)
	{
		String = str;
		TXBOX_text.setString(str);
	}
	sf::Text getText()
	{
		return TXBOX_text;
	}

	void setEnable(bool var)
	{
		IsEnable = var;
	}
	void switchEnable()
	{
		if (IsEnable == true)
		{
			IsEnable = false;
		}
		else
		{
			IsEnable = true;
		}
	}
	bool getIsEnable()
	{
		return IsEnable;
	}

	void setPosition(sf::Vector2u newPos)
	{
		TXBOX_text.setPosition(sf::Vector2f(static_cast<float>(newPos.x),
			static_cast<float>(newPos.y)));
	}
	sf::Vector2u getPosition()
	{
		sf::Vector2f text_position = TXBOX_text.getPosition();
		return sf::Vector2u(static_cast<size_t>(text_position.x),
			static_cast<size_t>(text_position.y));
	}
};