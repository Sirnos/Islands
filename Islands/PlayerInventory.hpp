#pragma once

#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>

#include "ItemField.hpp"

class PlayerInventory
{
	std::array<std::array<ItemField, PlayerInventorySize>, PlayerInventorySize> Inventory;
	std::array<ItemField, PlayerInventorySize> HandInventory;
	std::array<ItemField, 3> ArmorInventory;
	ItemField HoldItem;

	std::vector<ItemField> *InteractedChest;

public:
	PlayerInventory() = default;
	~PlayerInventory() { InteractedChest = nullptr; }

	ItemField getInventoryField(const sf::Vector2u &field)
	{
		return Inventory[field.x][field.y];
	}
	ItemField getHandInventoryField(unsigned field)
	{
		return HandInventory[field];
	}
	ItemField getArmorInventoryField(unsigned field)
	{
		return ArmorInventory[field];
	}
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
		if (!item.isCorrect())
		{
			item.empty();
		}
		InteractedChest->operator[](field) = item;
	}
	void popInteractionWithChest()
	{
		InteractedChest = nullptr;
	}
	bool isInteractedChestExist() const
	{
		if (InteractedChest == nullptr) { return false; }
		return true;
	}

	void setInventoryField(const sf::Vector2u &field, ItemField newVal)
	{
		if (!newVal.isCorrect())
		{
			newVal.empty();
		}
		Inventory[field.x][field.y] = newVal;
	}
	void setHandInventoryField(unsigned field, ItemField newVal)
	{
		if (!newVal.isCorrect())
		{
			newVal.empty();
		}
		HandInventory[field] = newVal;
	}
	void setArmorField(unsigned field, ItemField newVal)
	{
		if (!newVal.isCorrect())
		{
			newVal.empty();
		}
		ArmorInventory[field] = newVal;
	}
	void setHoldItem(ItemField newVal)
	{
		if (!newVal.isCorrect())
		{
			newVal.empty();
		}
		HoldItem = newVal;
	}

	void pushItem(ItemField &item, unsigned ItemMaxStack)
	{
		if (!item.isCorrect()) { item.empty(); return; }

		for (size_t i = 0; i < PlayerInventorySize; i++)
		{
			if (getHandInventoryField(i).ItemId == item.ItemId)
			{
				ItemField temp = getHandInventoryField(i);
				temp += item.ItemAmount;

				if (temp.ItemAmount >= ItemMaxStack)
				{
					item.ItemAmount = temp.ItemAmount - ItemMaxStack;
					temp.ItemAmount -= (temp.ItemAmount - ItemMaxStack);
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
					if (temp.ItemAmount >= ItemMaxStack)
					{
						item.ItemAmount = temp.ItemAmount - ItemMaxStack;
						temp.ItemAmount -= (temp.ItemAmount - ItemMaxStack);
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
