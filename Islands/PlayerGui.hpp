#pragma once

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "PlayerEquipmentGui.hpp"

class PlayerGui
{
	bool isEqGuiEnable;

public:
	PlayerEquipmentGui EquipmentGui;

	void create()
	{
		EquipmentGui.create();
	}
	//void pushKeyBinds(std::string keybinds){}

	void setNewPosition(sf::Vector2f newPosition) { EquipmentGui.setNewPosition(newPosition); }

	//void makeDrawableVector(std::vector<sf::Drawable*> &vector){}

	//void pushMouseState(sf::Mouse::Button button, sf::Vector2i mousePosition) {}

	void pushKeyState(char key){}

	void pushInteractionWithChestObjectType(/*chest* obj*/){}

	void switchEnable() { isEqGuiEnable = !isEqGuiEnable; }
};