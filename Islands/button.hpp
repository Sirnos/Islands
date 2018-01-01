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
	button() = default;
	button(const sf::Font &font, const sf::Vector2i &position, const sf::Vector2i &size, const std::string &text)
		:buttText(text, font), InteractionBoxPosition(position), InteractionBoxSize(size)
	{}
	~button() = default;

	const sf::Text &getText() const { return buttText; }
	sf::Vector2i getInterBoxSize() const { return InteractionBoxSize; }
	sf::Vector2i getInterBoxPos() const { return InteractionBoxPosition; }

	bool isClick(const sf::Vector2i &mousePos)
	{
		sf::IntRect ClickArea{ InteractionBoxPosition,InteractionBoxSize };
		if (ClickArea.contains(mousePos))
		{
			return true;
		}
		return false;
	}

	void setLogicBoxSize(const sf::Vector2i &newSize) { InteractionBoxSize = newSize; }
	void setLogicBoxPos(const sf::Vector2i &newPosition) { InteractionBoxPosition = newPosition; }
	void setButtonPosition(const sf::Vector2f &newPos) { buttText.setPosition(newPos); }
	void setTextOutlineColor(const sf::Color &col) { buttText.setOutlineColor(col); }
	void setTextFillColor(const sf::Color &col) { buttText.setFillColor(col); }
	void setTextStyle(const sf::Text::Style &style) { buttText.setStyle(style); }
	void setString(const std::string &str) { buttText.setString(str); }
	void setCharacterSize(unsigned Size) { buttText.setCharacterSize(Size); }
	void setFont(const sf::Font &fon) { buttText.setFont(fon); }

	void setColor(const sf::Color &col)
	{
		buttText.setFillColor(col);
		buttText.setOutlineColor(col);
	}

	void create(const sf::Vector2f &pos, const sf::Vector2i &Lpos, const sf::Vector2i &Lsize, const std::string &str, const sf::Font &fon)
	{
		buttText.setPosition(pos);
		buttText.setString(str);
		buttText.setFont(fon);
		InteractionBoxPosition = Lpos;
		InteractionBoxSize = Lsize;
	}
};