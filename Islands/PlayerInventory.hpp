#pragma once

#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>

#include "ItemField.hpp"

class PlayerInventory
{
	std::array<std::array<ItemField, PLAYER_INVENTORY_SIZE>, PLAYER_INVENTORY_SIZE> Inventory;
	std::array<ItemField, PLAYER_INVENTORY_SIZE> HandInventory;
	std::array<ItemField, PLAYER_ARMOR_INVENTORY_SIZE> ArmorInventory;
	ItemField HoldItem;

	std::vector<ItemField> *InteractedChest;

public:
	PlayerInventory() = default;
	~PlayerInventory() { InteractedChest = nullptr; }


	ItemField getInventoryField(const sf::Vector2u &field) const
	{
		return Inventory[field.x][field.y];
	}
	ItemField getHandInventoryField(unsigned field) const
	{
		return HandInventory[field];
	}
	ItemField getArmorInventoryField(unsigned field) const
	{
		return ArmorInventory[field];
	}
	ItemField getHoldItem() const
	{
		return HoldItem;
	}


	void pushInteractionWithChest(std::vector<ItemField> *Chest)
	{
		InteractedChest = Chest;
	}
	ItemField getItemFromInteractedChest(unsigned field) const
	{
		if (InteractedChest == nullptr) { return ItemField(); }
		return InteractedChest->operator[](field);
	}
	size_t getInteractedChestSize() const
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

		for (size_t i = 0; i < PLAYER_INVENTORY_SIZE; i++)
		{
			if (getHandInventoryField(i).Id == item.Id)
			{
				ItemField temp = getHandInventoryField(i);
				temp += item.Amount;

				if (temp.Amount >= ItemMaxStack)
				{
					item.Amount = temp.Amount - ItemMaxStack;
					temp.Amount -= (temp.Amount - ItemMaxStack);
				}
				else
				{
					item = ItemField();
				}
				setHandInventoryField(i, temp);
			}
			else if (getHandInventoryField(i).Id == 0)
			{
				setHandInventoryField(i, item);
				item.empty();
				return;
			}
			for (size_t j = 0; j < PLAYER_INVENTORY_SIZE; j++)
			{
				if (getInventoryField(sf::Vector2u(i, j)).Id == item.Id)
				{
					ItemField temp = getInventoryField(sf::Vector2u(i, j));
					temp += item.Amount;
					if (temp.Amount >= ItemMaxStack)
					{
						item.Amount = temp.Amount - ItemMaxStack;
						temp.Amount -= (temp.Amount - ItemMaxStack);
					}
					else
					{
						item = ItemField();
					}
					setInventoryField(sf::Vector2u(i, j), temp);
				}
				else if (getInventoryField(sf::Vector2u(i, j)).Id == 0)
				{
					setInventoryField(sf::Vector2u(i, j), item);
					item.empty();
					return;
				}
			}
		}
	}


	const std::array<std::array<ItemField, PLAYER_INVENTORY_SIZE>, PLAYER_INVENTORY_SIZE> &getMainInventory() const
	{
		return Inventory;
	}
	std::array<std::array<ItemField, PLAYER_INVENTORY_SIZE>, PLAYER_INVENTORY_SIZE> &getMainInventory()
	{
		return Inventory;
	}
	const std::array<ItemField, PLAYER_INVENTORY_SIZE> &getHandInventory() const
	{
		return HandInventory;
	}
	std::array<ItemField, PLAYER_INVENTORY_SIZE> &getHandInventory()
	{
		return HandInventory;
	}
	const std::array<ItemField, PLAYER_ARMOR_INVENTORY_SIZE> &getArmorInventory() const
	{
		return ArmorInventory;
	}
	std::array<ItemField, PLAYER_ARMOR_INVENTORY_SIZE> &getArmorInventory()
	{
		return ArmorInventory;
	}
};
