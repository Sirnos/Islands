#pragma once

#include "EquipmentFieldInfo.hpp"
#include "Object.hpp"

enum class EquipmentType
{
	Inventory,
	Armor,
	Belt
};

struct EquipmentGui
{
	EquipmentFieldInfo Equipment[PlayerFieldsNumber][PlayerFieldsNumber];
	EquipmentFieldInfo ArmorEquipment[3];

	bool isEnable;

	EquipmentGui() { isEnable = false; }
};

struct HudGui
{
	const sf::Vector2i HpInfoScreenPos = sf::Vector2i(20, 880);
	const sf::Vector2i MpInfoScreenPos = sf::Vector2i(20, 940);

	sf::Text HpInfo;
	sf::Text MpInfo;

	EquipmentFieldInfo Belt[PlayerFieldsNumber];

	unsigned ActiveBeltField;

	HudGui() { ActiveBeltField = 0; }

	void pushNewValuesForHpInfo(unsigned newMaxPlayerHp, unsigned newActualPlayerHp)
	{
		HpInfo.setString("Hp: " + std::to_string(newActualPlayerHp) + " / " + std::to_string(newMaxPlayerHp));
	}
	void pushNewValuesForMpInfo(unsigned newMaxPlayerMp, unsigned newActualPlayerMp)
	{
		MpInfo.setString("Mp: " + std::to_string(newActualPlayerMp) + " / " + std::to_string(newMaxPlayerMp));
	}
};

struct ChestGui
{
	std::vector<EquipmentFieldInfo> Chest;
};

struct Gui
{
	EquipmentGui Eq;
	HudGui Hud;

	sf::Font GuiFont;

	const int keyForEqGuiEnable = 'e';
	const int keyForBeltFields[PlayerFieldsNumber] = { '1','2','3','4','5' };

	Gui() 
	{ 
		GuiFont.loadFromFile("Data/Fonts/ariali.ttf");
		Hud.MpInfo.setFont(GuiFont);
		Hud.HpInfo.setFont(GuiFont);
		Hud.MpInfo.setFillColor(sf::Color::Blue);
		Hud.HpInfo.setFillColor(sf::Color::Red);
	}

	void pushKeyState(char key)
	{
		int keyVar = tolower(key);

		if (keyVar == keyForEqGuiEnable)
		{
			Eq.isEnable = !Eq.isEnable;
		}
		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			if (keyVar == keyForBeltFields[i])
			{
				Hud.ActiveBeltField = i-1;
				break;
			}
		}
	}

	void incrActiveBeltField()
	{
		if (Hud.ActiveBeltField >= PlayerFieldsNumber - 1)
			Hud.ActiveBeltField = 0;
		else
		{
			Hud.ActiveBeltField++;
		}

	}
	void decrActiveBeltField()
	{
		if (Hud.ActiveBeltField == 0)
			Hud.ActiveBeltField = PlayerFieldsNumber - 1;
		else
		{
			Hud.ActiveBeltField--;
		}
	}
};