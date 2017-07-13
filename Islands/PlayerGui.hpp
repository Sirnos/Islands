#pragma once

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Object.hpp"
#include "PlayerEquipmentGui.hpp"
#include "PlayerHud.hpp"

enum class EquipmentType
{
	Inventory,
	Armor,
	Belt
};

class PlayerGui
{
	sf::Font GuiFont;

	bool isEqGuiEnable;

	const int keyForEqGuiEnable = 'e';
	const int keyForBeltFields[PlayerFieldsNumber] = { '1','2','3','4','5' };
	unsigned SelectedBeltField = 0;

	ItemField holdedItem;

	ChestObject *interactedChest;

public:
	PlayerEquipmentGui EquipmentGui;
	PlayerHud HudGui;

	void create()
	{
		GuiFont.loadFromFile("Data/Fonts/ariali.ttf");
		EquipmentGui.create();
		HudGui.create(GuiFont);
	}
	//void pushKeyBinds(std::string keybinds){}

	void setNewPosition(sf::Vector2f newPosition) { EquipmentGui.setNewPosition(newPosition); }

	ItemField getHoldedItem()
	{
		return holdedItem;
	}
	void assingNewItemToHold(ItemField item)
	{
		holdedItem = item;
	}
	void clearHoldedItem()
	{
		holdedItem.clear();
	}

	void pushKeyState(char key)
	{
		int keyVar = tolower(key);

		if (keyVar == keyForEqGuiEnable)
		{
			switchEqGuiEnable();
		}
		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			if (keyVar == keyForBeltFields[i])
			{
				SelectedBeltField = i;
				break;
			}
		}
	}

	unsigned getNumberOfSelectedBeltField() { return SelectedBeltField; }
	void incrSelectedBeltField()
	{
		if (SelectedBeltField >= PlayerFieldsNumber - 1)
			SelectedBeltField = 0;
		else
		{
			SelectedBeltField++;
		}

	}
	void decrSelectedBeltField()
	{
		if (SelectedBeltField == 0)
			SelectedBeltField = PlayerFieldsNumber - 1;
		else
		{
			SelectedBeltField--;
		}
	}

	ItemField getChestItem(unsigned index)
	{
		if (interactedChest == nullptr)
		{
			return ItemField();
		}
		return interactedChest->Contain[index];
	}
	void setChestItem(unsigned index,ItemField item)
	{
		if (interactedChest == nullptr) { return; }

		interactedChest->Contain[index] = item;
	}
	void pushInteractionWithChest(ChestObject *chest)
	{
		if (chest == nullptr)
		{
			return;
		}

		interactedChest = chest;
	}
	void popInteractionWithChest()
	{
		interactedChest = nullptr;
	}
	bool isChestExist()
	{
		if (interactedChest == nullptr) { return false; }
		return true;
	}
	size_t getChestCapSize()
	{
		if (interactedChest == nullptr) { return 0; }
		return interactedChest->Contain.size();
	}

	void switchEqGuiEnable() { isEqGuiEnable = !isEqGuiEnable; }
	bool getIsEqGuiEnable() { return isEqGuiEnable; }
};