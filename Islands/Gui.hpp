#pragma once

#include "EquipmentFieldInfo.hpp"
#include "Object.hpp"

enum class EquipmentType
{
	Inventory,
	Armor,
	Belt,
	Chest
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
	std::vector<EquipmentFieldInfo> ChestContain;
};

class Gui
{
	const int static EqFieldSizeI = 64;
	const int static EqFieldMarginI = 16;
public:


	ChestGui Chest;
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

		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			if (i < 3)
			{
				Eq.ArmorEquipment[i].ScreenPosition = getScreenPositionForArmorEquipmentField(i);
			}
			Hud.Belt[i].ScreenPosition = getScreenPositionForBeltEquipmentField(i);
			for (size_t j = 0; j < PlayerFieldsNumber; j++)
			{
				Eq.Equipment[i][j].ScreenPosition = getScreenPositionForEquipmentField(sf::Vector2u(i, j));
			}
		}
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
				Hud.ActiveBeltField = i;
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
	void createChestFields(unsigned size)
	{
		sf::Vector2i ScreenPos = BeginChestFieldPosition;
		for (size_t i = 0; i < size; i++)
		{
			Chest.ChestContain.push_back(EquipmentFieldInfo());
			if (i != 0 && i % 5 == 0) 
			{ 
				ScreenPos.x = BeginChestFieldPosition.x;
				ScreenPos.y += (EqFieldMarginI + EqFieldSizeI);
			}
			Chest.ChestContain.back().ScreenPosition = ScreenPos;

			ScreenPos.x += (EqFieldSizeI + EqFieldMarginI);
		}
	}
	void deleteChestFields() { Chest.ChestContain.clear(); }

private:
	sf::Vector2i getScreenPositionForEquipmentField(sf::Vector2u field)
	{
		return sf::Vector2i(BeginEquipmentFieldPosition.x + (field.x * EqFieldMarginI) + (field.x * EqFieldSizeI),
			BeginEquipmentFieldPosition.y + (field.y * EqFieldMarginI) + (field.y * EqFieldSizeI));
	}

	sf::Vector2i getScreenPositionForArmorEquipmentField(unsigned field)
	{
		return sf::Vector2i(BeginArmorEquipmentFieldPosition.x,
			BeginArmorEquipmentFieldPosition.y + (field * EqFieldMarginI + (field * EqFieldSizeI)));
	}

	sf::Vector2i getScreenPositionForBeltEquipmentField(unsigned field)
	{
		return sf::Vector2i(BeginBeltEquipmentFieldPosition.x + (field * EqFieldMarginI) +
			(field * EqFieldSizeI), BeginBeltEquipmentFieldPosition.y);
	}

};