#pragma once

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "PlayerEquipmentGui.hpp"
#include "PlayerHud.hpp"

class PlayerGui
{
	sf::Font GuiFont;

	bool isEqGuiEnable;

	const int keyForEqGuiEnable = 'e';
	const int keyForBeltFields[PlayerFieldsNumber] = { '1','2','3','4','5' };
	unsigned SelectedBeltField = 0;
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

	//void makeDrawableVector(std::vector<sf::Drawable*> &vector){}

	//void pushMouseState(sf::Mouse::Button button, sf::Vector2i mousePosition) 

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

	//void pushInteractionWithChestObjectType(/*chest* obj*/){}

	void switchEqGuiEnable() { isEqGuiEnable = !isEqGuiEnable; }
	unsigned getNumberOfSelectedBeltField() { return SelectedBeltField; }

	bool getIsEqGuiEnable() { return isEqGuiEnable; }
};