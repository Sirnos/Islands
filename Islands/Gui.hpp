#pragma once

#include "CraftingGui.hpp"
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
	std::array<std::array<EquipmentFieldInfo, PlayerInventorySize>, PlayerInventorySize> Equipment;
	std::array<EquipmentFieldInfo, 3> ArmorEquipment;

	bool isEnable;

	EquipmentGui()
		:isEnable(false), Equipment(), ArmorEquipment()
	{}
	EquipmentGui(const EquipmentGui & other) = delete;
	~EquipmentGui() = default;
};

struct HudGui
{
	const sf::Vector2i HpInfoScreenPos = sf::Vector2i(20, 880);
	const sf::Vector2i MpInfoScreenPos = sf::Vector2i(20, 940);

	sf::Text HpInfo;
	sf::Text MpInfo;

	std::array<EquipmentFieldInfo, PlayerInventorySize> Belt;

	unsigned ActiveBeltField;

	HudGui()
		:ActiveBeltField(0), HpInfo(), MpInfo(), Belt()
	{}
	HudGui(const HudGui & other) = delete;
	~HudGui() = default;

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

	ChestGui()
		:ChestContain()
	{}
	ChestGui(const ChestGui& other) = delete;
	~ChestGui() = default;
};

class Gui
{
	const int static EqFieldSizeI = 64;
	const int static EqFieldMarginI = 16;

	const int static keyForCraftGui = 'c';
	const int static keyForEqGui = 'e';

	const std::array<int, PlayerInventorySize> keyForBeltFields = { '1','2','3','4','5' };
public:
	CraftingGui Craft;
	ChestGui Chest;
	EquipmentGui Eq;
	HudGui Hud;

	sf::Font GuiFont;

	Gui() 
	{ 
		GuiFont.loadFromFile("Data/Fonts/ariali.ttf");
		Hud.MpInfo.setFont(GuiFont);
		Hud.HpInfo.setFont(GuiFont);
		Hud.MpInfo.setFillColor(sf::Color::Blue);
		Hud.HpInfo.setFillColor(sf::Color::Red);

		for (size_t i = 0; i < PlayerInventorySize; i++)
		{
			if (i < 3)
			{
				Eq.ArmorEquipment[i].ScreenPosition = getScreenPositionForArmorEquipmentField(i);
			}
			Hud.Belt[i].ScreenPosition = getScreenPositionForBeltEquipmentField(i);
			for (size_t j = 0; j < PlayerInventorySize; j++)
			{
				Eq.Equipment[i][j].ScreenPosition = getScreenPositionForEquipmentField(sf::Vector2u(i, j));
			}
		}
		
		Craft.PushFontToCraftingInfo(GuiFont);
	}
	Gui(const Gui & other) = delete;
	~Gui() = default;

	void pushKeyState(char key)
	{
		int keyVar = tolower(key);

		if (keyVar == keyForEqGui)
		{
			Eq.isEnable = !Eq.isEnable;
			Craft.isEnable = false;

			return;
		}
		else if(keyVar == keyForCraftGui)
		{
			Craft.isEnable = !Craft.isEnable;
			Eq.isEnable = false;

			return;
		}

		for (size_t i = 0; i < PlayerInventorySize; i++)
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
		if (Hud.ActiveBeltField >= PlayerInventorySize - 1)
			Hud.ActiveBeltField = 0;
		else
		{
			Hud.ActiveBeltField++;
		}

	}
	void decrActiveBeltField()
	{
		if (Hud.ActiveBeltField == 0)
			Hud.ActiveBeltField = PlayerInventorySize - 1;
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