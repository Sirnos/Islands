#pragma once

#include <SFML/Graphics/Text.hpp>

#include "EquipmentField.hpp"

class PlayerHud
{
	sf::Text PlayerHpInfo;
	sf::Text PlayerMpInfo;

	EquipmentField Belt[PlayerFieldsNumber];

public:
	const sf::Vector2i BeltFieldPosOnScreen = sf::Vector2i(420, 880);
	const sf::Vector2i HpInfoScreenPos = sf::Vector2i(20, 880);
	const sf::Vector2i MpInfoScreenPos = sf::Vector2i(20, 940);

	void create(sf::Font &fontForHud)
	{
		PlayerHpInfo.setFillColor(sf::Color::Red);
		PlayerMpInfo.setFillColor(sf::Color::Blue);

		PlayerHpInfo.setFont(fontForHud);
		PlayerMpInfo.setFont(fontForHud);

		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			Belt[i].create(sf::Vector2f(), DefaultEqFieldSize, DefaultEqFieldColor);
		}
	}
	void setNewPosition(sf::Vector2f newPosForHpInfo, sf::Vector2f newPosForMpInfo,sf::Vector2f newPosForBelt)
	{
		PlayerHpInfo.setPosition(newPosForHpInfo);
		PlayerMpInfo.setPosition(newPosForMpInfo);

		auto position = newPosForBelt;
		const sf::Vector2f space = sf::Vector2f(DefaultEqFieldSize + 20, 0);

		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			Belt[i].setPosition(position);
			position += space;
		}
	}

	void pushTextureToBeltField(unsigned field, sf::Texture *txt)
	{
		Belt[field].TextureRect.setTexture(txt,true);
	}

	void pushNewValuesForHpInfo(unsigned newMaxPlayerHp, unsigned newActualPlayerHp)
	{
		PlayerHpInfo.setString("Hp: " + std::to_string(newActualPlayerHp) + " / " + std::to_string(newMaxPlayerHp));
	}
	void pushNewValuesForMpInfo(unsigned newMaxPlayerMp, unsigned newActualPlayerMp)
	{
		PlayerMpInfo.setString("Mp: " + std::to_string(newActualPlayerMp) + " / " + std::to_string(newMaxPlayerMp));
	}

	//which == true then return PlayerMpInfo else  return PlayerHpInfo
	sf::Text& getHudElement(bool which)
	{
		if (which == true) { return PlayerMpInfo; }

		return PlayerHpInfo;
	}

	sf::RectangleShape &getBeltFieldRect(unsigned field) { return Belt[field].FieldRect; }
	sf::RectangleShape &getBeltFieldTextureRect(unsigned field) { return Belt[field].TextureRect; }

	void clearBeltField(unsigned field)
	{
		Belt[field].FieldRect.setFillColor(DefaultEqFieldColor);
		Belt[field].TextureRect.setTexture(nullptr);
	}

	void setHoverForBeltField(unsigned field, bool newVal)
	{
		Belt[field].ishover = newVal;
	}
	bool getHoverFromBeltField(unsigned field)
	{
		return Belt[field].ishover;
	}

};