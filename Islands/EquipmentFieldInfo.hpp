#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "ItemField.hpp"

const sf::Vector2i BeginBeltEquipmentFieldPosition = sf::Vector2i(420, 880);
const sf::Vector2i BeginEquipmentFieldPosition = sf::Vector2i(200, 100);
const sf::Vector2i BeginArmorEquipmentFieldPosition = sf::Vector2i(80, 100);
const float EquipmentFieldMargin = 16.0f;
const float EquipmentFieldSize = 64.0f;
const sf::Color EquipmentFieldColor = sf::Color(120, 250, 120, 250);
const sf::Color HoverEquipmentFieldColor = sf::Color(250, 60, 80, 250);

const int EqFieldSizeI = 64;
const int EqFieldMarginI = 16;

inline sf::Vector2i getScreenPositionForEquipmentField(sf::Vector2u field)
{
	return sf::Vector2i(BeginEquipmentFieldPosition.x + (field.x * EqFieldMarginI) + (field.x * EqFieldSizeI),
		BeginEquipmentFieldPosition.y + (field.y * EqFieldMarginI) + (field.y * EqFieldSizeI));
}

inline sf::Vector2i getScreenPositionForArmorEquipmentField(unsigned field)
{
	return sf::Vector2i(BeginArmorEquipmentFieldPosition.x,
		BeginArmorEquipmentFieldPosition.y + (field * EqFieldMarginI + (field * EqFieldSizeI)));
}

inline sf::Vector2i getScreenPositionForBeltEquipmentField(unsigned field)
{
	return sf::Vector2i(BeginBeltEquipmentFieldPosition.x + (field * EqFieldMarginI) +
		(field * EqFieldSizeI), BeginBeltEquipmentFieldPosition.y);
}

struct EquipmentFieldInfo
{
	sf::Vector2f Position;
	bool isHover;

	void pushChangesToRectangleShape(sf::RectangleShape &rect)
	{
		rect.setPosition(Position);
		if (isHover) { rect.setFillColor(HoverEquipmentFieldColor); }
		else { rect.setFillColor(EquipmentFieldColor); }
	}
	void checkIsHover(sf::Vector2f mousePos)
	{
		isHover = CollisionDetect::isPointInRectangle(mousePos, Position, sf::Vector2f(EquipmentFieldSize, EquipmentFieldSize));
	}

	EquipmentFieldInfo()
	{
		Position = sf::Vector2f(0, 0);
		isHover = false;
	}
};