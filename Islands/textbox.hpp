#pragma once

#include <SFML/Graphics/Text.hpp>

class textbox
{
	size_t MAX_TEXT_SIZE;

	bool IsEnable;
	bool IsConstant;
	std::string String;
	sf::Text TXBOX_text;

public:
	textbox()
		:MAX_TEXT_SIZE(32), IsEnable(false), IsConstant(false)
	{}
	textbox(size_t textLength, const std::string &text, const sf::Font &font, bool Constant)
		:MAX_TEXT_SIZE(textLength), TXBOX_text(text, font), String(text), IsConstant(Constant), IsEnable(false)
	{}
	textbox(const textbox &other)
		:MAX_TEXT_SIZE(other.MAX_TEXT_SIZE), IsEnable(other.IsEnable), IsConstant(other.IsConstant), String(other.String), TXBOX_text(other.TXBOX_text)
	{}
	~textbox() = default;

	//Return String when player press Return(Enter)
	//Else return empty string
	std::string operator()(const sf::Event &event)
	{
		if (IsEnable == false || IsConstant == true)
		{
			return "";
		}
		else
		{
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 13)
				{
					std::string tmp(String.data());
					String.clear();
					TXBOX_text.setString(String);
					return tmp;
				}

				if (event.text.unicode == 8)
				{
					if (String.size() > 0 == true)
					{
						String.pop_back();
						TXBOX_text.setString(String);
						return "";
					}
				}

				if (String.size() >= MAX_TEXT_SIZE)
				{
					return "";
				}

				if (event.text.unicode < 128)
				{
					String += static_cast<char>(event.text.unicode);
					TXBOX_text.setString(String);
				}
			}
		}
		return "";
	}

	void create(const sf::Font &font, bool isConstant, const std::string &strForTextBox, size_t MAXSIZE)
	{
		TXBOX_text.setFont(font);
		IsConstant = isConstant;
		TXBOX_text.setString(strForTextBox);
		MAX_TEXT_SIZE = MAXSIZE;
	}
	void changeVars(const sf::Color &colorForText, unsigned int CharSize, sf::Uint32 style)
	{
		TXBOX_text.setFillColor(colorForText);
		TXBOX_text.setOutlineColor(colorForText);
		TXBOX_text.setCharacterSize(CharSize);
		TXBOX_text.setStyle(style);
	}
	bool isClick(const sf::Vector2f &mousePos, bool isMouseClick)
	{
		return (TXBOX_text.getGlobalBounds().contains(mousePos) && isMouseClick == true);
	}

	std::string getString() { return TXBOX_text.getString(); }
	void setString(const std::string &str)
	{
		String = str;
		TXBOX_text.setString(str);
	}
	sf::Text getText() { return TXBOX_text; }

	void setEnable(bool var) { IsEnable = var; }
	void switchEnable()
	{
		IsEnable = !IsEnable;
	}
	bool getIsEnable() const { return IsEnable; }

	void setPosition(const sf::Vector2f &newPos)
	{
		TXBOX_text.setPosition(newPos);
	}
	sf::Vector2u getPosition() const
	{
		sf::Vector2f text_position = TXBOX_text.getPosition();
		return sf::Vector2u(static_cast<size_t>(text_position.x),
			static_cast<size_t>(text_position.y));
	}
};