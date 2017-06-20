#pragma once

const unsigned PlayerFieldsNumber = 5;

struct ItemField
{
	unsigned ItemId;
	unsigned ItemAmount;

	ItemField() { ItemId = 0; ItemAmount = 0; }
	ItemField(unsigned newItemId, unsigned newItemAmount)
		:ItemId(newItemId),
		ItemAmount(newItemAmount)
	{}
	ItemField(ItemField &other)
	{
		this->operator=(other);
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

	bool isClear() 
	{
		if (ItemId == 0 || ItemAmount == 0)
		{
			return true;
		}
		return false;
	}
	void clear() { ItemId = 0; ItemAmount = 0; }
};