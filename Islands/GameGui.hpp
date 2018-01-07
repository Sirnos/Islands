#pragma once

#include "CraftingGui.hpp"
#include "Object.hpp"

namespace Gui
{
	const static std::string DEFAULT_GAME_GUI_FONT_LOCATION = "Data/Fonts/ariali.ttf";

	enum class EquipmentType
	{
		Inventory,
		Armor,
		Belt,
		Chest
	};

	struct EquipmentGui
	{
		std::array<std::array<EquipmentFieldInfo, PLAYER_INVENTORY_SIZE>, PLAYER_INVENTORY_SIZE> Equipment;
		std::array<EquipmentFieldInfo, PLAYER_ARMOR_INVENTORY_SIZE> ArmorEquipment;

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

		std::array<EquipmentFieldInfo, PLAYER_INVENTORY_SIZE> Belt;

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

	class GameGui
	{
		const int static EqFieldSizeI = 64;
		const int static EqFieldMarginI = 16;

		const int static keyForCraftGui = 'c';
		const int static keyForEqGui = 'e';

		const std::array<int, PLAYER_INVENTORY_SIZE> keyForBeltFields = { '1','2','3','4','5' };
	public:
		CraftingGui Craft;
		ChestGui Chest;
		EquipmentGui Eq;
		HudGui Hud;

		sf::Font GuiFont;

		GameGui()
		{
			GuiFont.loadFromFile(DEFAULT_GAME_GUI_FONT_LOCATION);
			Hud.MpInfo.setFont(GuiFont);
			Hud.HpInfo.setFont(GuiFont);
			Hud.MpInfo.setFillColor(sf::Color::Blue);
			Hud.HpInfo.setFillColor(sf::Color::Red);

			for (size_t i = 0; i < PLAYER_INVENTORY_SIZE; i++)
			{
				if (i < PLAYER_ARMOR_INVENTORY_SIZE)
				{
					Eq.ArmorEquipment[i].ScreenPosition = getScreenPositionForArmorEquipmentField(i);
				}
				Hud.Belt[i].ScreenPosition = getScreenPositionForBeltEquipmentField(i);
				for (size_t j = 0; j < PLAYER_INVENTORY_SIZE; j++)
				{
					Eq.Equipment[i][j].ScreenPosition = getScreenPositionForEquipmentField(sf::Vector2u(i, j));
				}
			}

			Craft.PushFontToCraftingInfo(GuiFont);
		}
		GameGui(const GameGui & other) = delete;
		~GameGui() = default;

		void pushKeyState(char key)
		{
			int keyVar = tolower(key);

			if (keyVar == keyForEqGui)
			{
				Eq.isEnable = !Eq.isEnable;
				Craft.isEnable = false;

				return;
			}
			else if (keyVar == keyForCraftGui)
			{
				Craft.isEnable = !Craft.isEnable;
				Eq.isEnable = false;

				return;
			}

			for (size_t i = 0; i < PLAYER_INVENTORY_SIZE; i++)
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
			if (Hud.ActiveBeltField >= PLAYER_INVENTORY_SIZE - 1)
				Hud.ActiveBeltField = 0;
			else
			{
				Hud.ActiveBeltField++;
			}

		}
		void decrActiveBeltField()
		{
			if (Hud.ActiveBeltField == 0)
				Hud.ActiveBeltField = PLAYER_INVENTORY_SIZE - 1;
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
		sf::Vector2i getScreenPositionForEquipmentField(const sf::Vector2u &field)
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
}
