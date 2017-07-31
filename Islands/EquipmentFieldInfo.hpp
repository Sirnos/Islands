#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "ItemField.hpp"

const sf::Vector2i BeginBeltEquipmentFieldPosition = sf::Vector2i(420, 880);
const sf::Vector2i BeginEquipmentFieldPosition = sf::Vector2i(200, 100);
const sf::Vector2i BeginArmorEquipmentFieldPosition = sf::Vector2i(80, 100);
const sf::Vector2i BeginChestFieldPosition = sf::Vector2i(800,100);

const sf::Vector2i BeginCraftFieldPosition = sf::Vector2i(80, 80);

const float EquipmentFieldSize = 64.0f;
const sf::Color EquipmentFieldColor = sf::Color(120, 250, 120, 250);
const sf::Color HoverEquipmentFieldColor = sf::Color(250, 60, 80, 250);
const sf::Color ActiveBeltFieldColor = sf::Color(255, 40, 60, 250);

struct EquipmentFieldInfo
{
	sf::Vector2i ScreenPosition;
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