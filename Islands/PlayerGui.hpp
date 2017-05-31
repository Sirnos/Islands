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

	//void pushMouseState(sf::Mouse::Button button, sf::Vector2i mousePosition) {}

	void pushKeyState(char key)
	{
		int keyVar = tolower(key);

		if (keyVar == keyForEqGuiEnable)
		{
			switchEqGuiEnable();
		}
	}

	void pushInteractionWithChestObjectType(/*chest* obj*/){}

	void switchEqGuiEnable() { isEqGuiEnable = !isEqGuiEnable; }
	bool getIsEqGuiEnable() { return isEqGuiEnable; }
};