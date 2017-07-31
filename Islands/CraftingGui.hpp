#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "EquipmentFieldInfo.hpp"
#include "button.hpp"
#include <array>


struct CraftingInfo
{
	sf::RectangleShape Window;
	std::vector<sf::Text> InfoText;
};

struct CraftingGui
{
	static const int RecipeFieldMargin = 16;
	static const int RecipeFieldSize = 64;

	bool isEnable;

	std::array<std::array<EquipmentFieldInfo, 10>, 4> RecipeFields;
	CraftingInfo RecipeInfo;
	button CraftButton;

	CraftingGui()
	{
		isEnable = false;
		sf::Vector2i pos = BeginCraftFieldPosition;
		for (size_t i = 0; i < RecipeFields.size(); i++)
		{
			for (size_t j = 0; j < RecipeFields[i].size(); j++)
			{
				RecipeFields[i][j].ScreenPosition = pos;

				pos.y += (RecipeFieldMargin + RecipeFieldSize);
			}
			pos.y = BeginCraftFieldPosition.y;
			pos.x += (RecipeFieldMargin + RecipeFieldSize);
		}
	}
};