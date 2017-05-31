#pragma once

#include <SFML/Graphics/Text.hpp>

class PlayerHud
{
	sf::Text PlayerHpInfo;
	sf::Text PlayerMpInfo;

public:
	const sf::Vector2i HpInfoScreenPos = sf::Vector2i(20, 880);
	const sf::Vector2i MpInfoScreenPos = sf::Vector2i(20, 940);

	void create(sf::Font &fontForHud)
	{
		PlayerHpInfo.setFillColor(sf::Color::Red);
		PlayerMpInfo.setFillColor(sf::Color::Blue);

		PlayerHpInfo.setFont(fontForHud);
		PlayerMpInfo.setFont(fontForHud);
	}
	void setNewPosition(sf::Vector2f newPosForHpInfo, sf::Vector2f newPosForMpInfo)
	{
		PlayerHpInfo.setPosition(newPosForHpInfo);
		PlayerMpInfo.setPosition(newPosForMpInfo);
	}

	void pushNewValuesForHpInfo(unsigned newMaxPlayerHp, unsigned newActualPlayerHp)
	{
		PlayerHpInfo.setString("Hp: " + std::to_string(newActualPlayerHp) + " / " + std::to_string(newMaxPlayerHp));
	}
	void pushNewValuesForMpInfo(unsigned newMaxPlayerMp, unsigned newActualPlayerMp)
	{
		PlayerMpInfo.setString("Mp: " + std::to_string(newActualPlayerMp) + " / " + std::to_string(newMaxPlayerMp));
	}

	//which == true then return PlayerMpInfo else PlayerHpInfo
	sf::Text* getHudElement(bool which)
	{
		if (which == true) { return &PlayerMpInfo; }

		return &PlayerHpInfo;
	}
};