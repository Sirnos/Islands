#pragma once

#include "Character.hpp"
#include "ItemField.hpp"


class playerCharacter :public Character
{
	ItemField Inventory[PlayerFieldsNumber][PlayerFieldsNumber];
	ItemField HandInventory[PlayerFieldsNumber];
	ItemField ArmorFields[3];

	sf::Vector2f spawnPoint;
public:
	playerCharacter()
		:Character()
	{
		Inventory[0][0] = ItemField(2, 20);
		Inventory[1][1] = ItemField(3, 40);
		Inventory[3][3] = ItemField(11, 140);
		Inventory[4][4] = ItemField(11, 120);

		HandInventory[1] = ItemField(10, 20);
		HandInventory[2] = ItemField(3, 10);
	}
	playerCharacter(sf::Texture *texture, sf::Vector2f position,sf::Vector2f stats)
		:Character(stats)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(10, 10));
	}

	void move(sf::Vector2f moveVector) { CharacterShape.move(moveVector); }
	sf::RectangleShape *getShape() { return &CharacterShape; }
	void set(sf::Texture *texture, sf::Vector2f position,sf::Vector2f stats)
	{
		CharacterShape.setPosition(position);
		CharacterShape.setTexture(texture);
		CharacterShape.setSize(sf::Vector2f(40, 60));
		HP = stats.x;
		MP = stats.y;
	}

	void setSpawnPoint(sf::Vector2f newSpawnPoint) { spawnPoint = newSpawnPoint; }
	sf::Vector2f getSpawnPoint() { return spawnPoint; }

	ItemField getInventoryField(sf::Vector2u field)
	{
		return Inventory[field.x][field.y];
	}
	ItemField getHandInventoryField(unsigned field)
	{
		return HandInventory[field];
	}
	ItemField getArmorInventoryField(unsigned field) { return ArmorFields[field]; }

	void setInventoryField(sf::Vector2u field, ItemField newVal)
	{
		Inventory[field.x][field.y] = newVal;
	}
	void setHandInventoryField(unsigned field, ItemField newVal)
	{
		HandInventory[field] = newVal;
	}
	void setArmorField(unsigned field, ItemField newVal)
	{
		ArmorFields[field] = newVal;
	}

	void pushItemToPlayer(ItemField &item,ItemDefContainer &Items)
	{
		for (size_t i = 0; i < PlayerFieldsNumber; i++)
		{
			if (getHandInventoryField(i).ItemId == item.ItemId)
			{
				ItemField temp = getHandInventoryField(i);
				temp += item.ItemAmount;
				unsigned maxStack = Items.getDefinition(item.ItemId)->getMaxStack();

				if (temp.ItemAmount >= maxStack)
				{
					item.ItemAmount = temp.ItemAmount - maxStack;
					temp.ItemAmount -= (temp.ItemAmount - maxStack);
					setHandInventoryField(i, temp);
				}
			}
			else if (getHandInventoryField(i).ItemId == 0)
			{
				setHandInventoryField(i, item);
				item.clear();
				return;
			}
			for (size_t j = 0; j < PlayerFieldsNumber; j++)
			{
				if (getInventoryField(sf::Vector2u(i, j)).ItemId == item.ItemId)
				{
					ItemField temp = getInventoryField(sf::Vector2u(i, j));
					temp += item.ItemAmount;
					unsigned maxStack = Items.getDefinition(item.ItemId)->getMaxStack();
					if (temp.ItemAmount >= maxStack)
					{
						item.ItemAmount = temp.ItemAmount - maxStack;
						temp.ItemAmount -= (temp.ItemAmount - maxStack);
						setInventoryField(sf::Vector2u(i, j), temp);
					}
				}
				else if (getInventoryField(sf::Vector2u(i, j)).ItemId == 0)
				{
					setInventoryField(sf::Vector2u(i, j), item);
					item.clear();
					return;
				}
			}
		}
	}
};