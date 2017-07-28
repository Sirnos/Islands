#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "EquipmentFieldInfo.hpp"
#include "button.hpp"


struct CraftingInfo
{
	sf::RectangleShape Window;
	std::vector<sf::Text> InfoText;
};



class CraftingGui
{
	static const int RecipeFieldMargin = 16;
	static const int RecipeFieldSize = 64;

public:
	std::vector<EquipmentFieldInfo> RecipeFields;
	CraftingInfo RecipeInfo;
	button CraftButton;
};