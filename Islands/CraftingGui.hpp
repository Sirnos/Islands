#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "EquipmentFieldInfo.hpp"
#include "button.hpp"
#include <array>


struct CraftingInfo
{
	sf::Vector2i WindowScreenPosition;

	sf::RectangleShape Window;
	std::vector<sf::Text> InfoText;

	sf::Text CraftAmountInfo;

	button CraftButton;

	button CraftAmountAddOne;
	button CraftAmountSubOne;

	button CraftAmountAddTen;
	button CraftAmountSubTen;

	CraftingInfo()
	{
		Window.setFillColor(EquipmentFieldColor);
		Window.setSize(sf::Vector2f(200, 400));
	}
};

struct CraftingGui
{
	static const int RecipeFieldMargin = 16;
	static const int RecipeFieldSize = 64;

	bool isEnable;

	std::array<std::array<EquipmentFieldInfo, 10>, 4> RecipeFields;
	CraftingInfo RecipeInfo;

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
		pos.x += (RecipeFieldSize);
		RecipeInfo.WindowScreenPosition = pos;
	}
	void PushFontToCraftingInfo(sf::Font &font)
	{
		RecipeInfo.CraftButton.create(sf::Vector2f(), RecipeInfo.WindowScreenPosition + sf::Vector2i(28,360), sf::Vector2i(128, 64), "Craft", font);
		RecipeInfo.CraftButton.setColor(HoverEquipmentFieldColor);
	}
};