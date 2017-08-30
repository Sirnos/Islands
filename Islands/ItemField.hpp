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

	ItemField &operator= (const ItemField &other)
	{
		ItemId = other.ItemId;
		ItemAmount = other.ItemAmount;

		return *this;
	}
	void operator+= (unsigned amount)
	{
		ItemAmount += amount;
	}
	void operator-= (unsigned amount)
	{
		ItemAmount -= amount;
	}

	bool operator==(const ItemField &other)
	{
		if (ItemId == other.ItemId) { return true; }
		return false;
	}
	bool operator!=(const ItemField &other)
	{
		return !(operator==(other));
	}

	bool isEmpty() 
	{
		if (ItemId == 0 || ItemAmount == 0)
		{
			return true;
		}
		return false;
	}
	bool isCorrect()
	{
		if (ItemAmount == 0 && ItemId != 0)
		{
			return false;
		}
		return true;
	}
	void empty() { *this = ItemField(); }
};