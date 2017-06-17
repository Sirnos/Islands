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

	void operator= (ItemField &other)
	{
		ItemId = other.ItemId;
		ItemAmount = other.ItemAmount;
	}

	void clear() { ItemId = 0; ItemAmount = 0; }
};