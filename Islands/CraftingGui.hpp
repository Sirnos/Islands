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
	sf::Vector2i CraftamountInfoScreenPos;

	button CraftButton;

	button CraftAmountAddOne;
	button CraftAmountSubOne;

	button CraftAmountAddTen;
	button CraftAmountSubTen;

	CraftingInfo()
	{
		Window.setFillColor(EquipmentFieldColor);
		Window.setSize(sf::Vector2f(200, 400));

		CraftButton.setColor(HoverEquipmentFieldColor);
		CraftAmountAddOne.setColor(HoverEquipmentFieldColor);
		CraftAmountSubOne.setColor(HoverEquipmentFieldColor);
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
		RecipeInfo.CraftButton.create(sf::Vector2f(), RecipeInfo.WindowScreenPosition + sf::Vector2i(48,360), sf::Vector2i(128, 64), "Craft", font);
		RecipeInfo.CraftAmountInfo.setFont(font);
		RecipeInfo.CraftAmountAddOne.create(sf::Vector2f(), RecipeInfo.WindowScreenPosition + sf::Vector2i(96, 264), sf::Vector2i(64, 48), "+1", font);
		RecipeInfo.CraftAmountSubOne.create(sf::Vector2f(), RecipeInfo.WindowScreenPosition + sf::Vector2i(24, 264), sf::Vector2i(64, 48), "-1", font);
		RecipeInfo.CraftamountInfoScreenPos = RecipeInfo.WindowScreenPosition + sf::Vector2i(48, 316);
	}
};