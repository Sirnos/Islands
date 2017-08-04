#pragma once

#include "Character.hpp"
#include "ItemField.hpp"
#include <array>


class playerCharacter :public Character
{
	std::array<std::array<ItemField, PlayerInventorySize>, PlayerInventorySize> Inventory;
	std::array<ItemField, PlayerInventorySize> HandInventory;
	std::array<ItemField, 3> ArmorInventory;

	ItemField HoldItem;

	std::vector<ItemField> *InteractedChest;

	sf::Vector2f spawnPoint;
public:
	playerCharacter()
		:Character()
	{}
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
	ItemField getArmorInventoryField(unsigned field) { return ArmorInventory[field]; }
	ItemField getHoldItem() { return HoldItem; }

	void pushInteractionWithChest(std::vector<ItemField> *Chest)
	{
		InteractedChest = Chest;
	}
	ItemField getItemFromInteractedChest(unsigned field)
	{
		if (InteractedChest == nullptr) { return ItemField(); }
		return InteractedChest->operator[](field);
	}
	size_t getInteractedChestSize()
	{
		if (InteractedChest == nullptr) { return 0; }
		return InteractedChest->size();
	}
	void setInteractedChestItemField(unsigned field, ItemField item)
	{
		InteractedChest->operator[](field) = item;
	}
	void popInteractionWithChest()
	{
		InteractedChest = nullptr;
	}
	bool isInteractedChestExist()
	{
		if (InteractedChest == nullptr) { return false; }
		return true;
	}

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
		ArmorInventory[field] = newVal;
	}
	void setHoldItem(ItemField newVal) { HoldItem = newVal; }

	void pushItemToPlayer(ItemField &item,ItemDefContainer &Items)
	{
		for (size_t i = 0; i < PlayerInventorySize; i++)
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
				}
				else
				{
					item = ItemField();
				}
				setHandInventoryField(i, temp);
			}
			else if (getHandInventoryField(i).ItemId == 0)
			{
				setHandInventoryField(i, item);
				item.empty();
				return;
			}
			for (size_t j = 0; j < PlayerInventorySize; j++)
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
					}
					else
					{
						item = ItemField();
					}
					setInventoryField(sf::Vector2u(i, j), temp);
				}
				else if (getInventoryField(sf::Vector2u(i, j)).ItemId == 0)
				{
					setInventoryField(sf::Vector2u(i, j), item);
					item.empty();
					return;
				}
			}
		}
	}
};