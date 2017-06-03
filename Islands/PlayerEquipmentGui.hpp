#pragma once

#include "ItemField.hpp"

#include <SFML/Graphics/RectangleShape.hpp>


struct EquipmentField
{
	ItemField item;

	sf::RectangleShape FieldRect;
	sf::RectangleShape TextureRect;

	void create(sf::Vector2f atPosition, float fieldSize, sf::Color fieldColor)
	{
		FieldRect.setPosition(atPosition);
		FieldRect.setSize(sf::Vector2f(fieldSize,fieldSize));
		FieldRect.setFillColor(fieldColor);

		TextureRect.setPosition(atPosition);
		TextureRect.setSize(sf::Vector2f(fieldSize, fieldSize));
	}
	void setPosition(sf::Vector2f newPosition)
	{
		FieldRect.setPosition(newPosition);
		TextureRect.setPosition(newPosition);
	}

	void assingItem(ItemField other,sf::Texture *texture)
	{
		item.assingFromOther(other);
		TextureRect.setTexture(texture);
	}
	void clearField(sf::Color color)
	{
		item.clear();
		TextureRect.setTexture(nullptr);
	}
};


class PlayerEquipmentGui
{
	const float EquipmentFieldSize = 64;
	const sf::Color EquipmentFieldColor = sf::Color(120, 250, 120, 250);
	const sf::Color EqArmorFieldColor = sf::Color(250, 0, 0, 250);


	EquipmentField Fields[PlayerFieldsNumber][PlayerFieldsNumber];
	EquipmentField ArmorFields[3];

	sf::Vector2f EquipmentGuiPosition;


public:
	const sf::Vector2i defaultEquipmentGuiPosOnScreen = sf::Vector2i(200, 100);

	void create()
	{
		for (unsigned i = 0; i < PlayerFieldsNumber; i++)
		{
			if (i < 3)
			{
				ArmorFields[i].create(sf::Vector2f(), EquipmentFieldSize, EqArmorFieldColor);
			}
			for (unsigned j = 0; j < PlayerFieldsNumber; j++)
			{
				Fields[i][j].create(sf::Vector2f(), EquipmentFieldSize, EquipmentFieldColor);
			}
		}

	}
	void setNewPosition(sf::Vector2f position) 
	{ 
		EquipmentGuiPosition = position;

		sf::Vector2f Pos = position;

		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			Pos.x = position.x + 20;
			Pos.y += 20;
			for (size_t j = 0; j < PlayerFieldsNumber; j++)
			{
				Fields[i][j].setPosition(Pos);

				Pos.x += 64;
				Pos.x += 20;
			}
			Pos.y += 64;
		}
		sf::Vector2f ArmorFieldPos = position;
		ArmorFieldPos -= sf::Vector2f(88, -20);
		for (unsigned i = 0; i < 3; i++)
		{
			ArmorFields[i].setPosition(ArmorFieldPos);

			ArmorFieldPos.y += 88;
		}
	}

	sf::RectangleShape &getFieldRect(sf::Vector2u field) { return Fields[field.x][field.y].FieldRect; }
	sf::RectangleShape &getArmorFieldRect(unsigned field) { return ArmorFields[field].FieldRect; }
	ItemField getItemField(sf::Vector2u field) { return Fields[field.x][field.y].item; }
	ItemField getArmorItemField(unsigned field) { return ArmorFields[field].item; }
};