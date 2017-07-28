#pragma once

const unsigned PlayerInventorySize = 5;

struct ItemField
{
	unsigned ItemId;
	unsigned ItemAmount;

	ItemField() { ItemId = 0; ItemAmount = 0; }
	ItemField(unsigned newItemId, unsigned newItemAmount)
		:ItemId(newItemId),
		ItemAmount(newItemAmount)
	{}
	ItemField(const ItemField &other)
	{
		ItemId = other.ItemId;
		ItemAmount = other.ItemAmount;
	}

	void operator= (ItemField &other)
	{
		ItemId = other.ItemId;
		ItemAmount = other.ItemAmount;
	}
	void operator+= (unsigned amount)
	{
		ItemAmount += amount;
	}
	void operator-= (unsigned amount)
	{
		ItemAmount -= amount;
	}

	bool isEmpty() 
	{
		if (ItemId == 0 || ItemAmount == 0)
		{
			return true;
		}
		return false;
	}
	void empty() { ItemId = 0; ItemAmount = 0; }
};