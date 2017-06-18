#pragma once

#include "EquipmentField.hpp"

class PlayerEquipmentGui
{
	EquipmentField Fields[PlayerFieldsNumber][PlayerFieldsNumber];
	EquipmentField ArmorFields[3];

	sf::Vector2f EquipmentGuiPosition;

public:
	const sf::Vector2i defaultEquipmentGuiPosOnScreen = sf::Vector2i(200, 100);

	void pushTextureToFields(sf::Vector2u field,sf::Texture *txt)
	{
		Fields[field.x][field.y].TextureRect.setTexture(txt);
	}
	void pushTextureToArmorFields(unsigned field, sf::Texture *txt)
	{
		ArmorFields[field].TextureRect.setTexture(txt);
	}

	void create()
	{
		for (unsigned i = 0; i < PlayerFieldsNumber; i++)
		{
			if (i < 3)
			{
				ArmorFields[i].create(sf::Vector2f(), DefaultEqFieldSize, DefaultEqFieldColor);
			}
			for (unsigned j = 0; j < PlayerFieldsNumber; j++)
			{
				Fields[i][j].create(sf::Vector2f(), DefaultEqFieldSize, DefaultEqFieldColor);
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
	sf::RectangleShape &getTextureRect(sf::Vector2u field) { return Fields[field.x][field.y].TextureRect; }
	sf::RectangleShape &getArmorFieldRect(unsigned field) { return ArmorFields[field].FieldRect; }
	sf::RectangleShape &getArmorTextureRect(unsigned field) { return ArmorFields[field].TextureRect; }

	void setHoverForInventoryField(sf::Vector2u field, bool newVal)
	{
		Fields[field.x][field.y].ishover = newVal;
	}
	bool getHoverFromInventoryField(sf::Vector2u field)
	{
		return Fields[field.x][field.y].ishover;
	}

	void setHoverForArmorField(unsigned field, bool newVal)
	{
		ArmorFields[field].ishover = newVal;
	}
	bool getHoverFromArmorField(unsigned field)
	{
		return ArmorFields[field].ishover;
	}

};