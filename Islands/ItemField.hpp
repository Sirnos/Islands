#pragma once

const unsigned PlayerFieldsNumber = 5;

struct ItemField
{
	unsigned ItemId = 0;
	unsigned ItemAmount = 0;

	void clear() { ItemId = 0; ItemAmount = 0; }
	void assignNew(unsigned newItemId, unsigned newItemAmount) { ItemId = newItemId; ItemAmount = newItemAmount; }
	void assingFromOther(ItemField other)
	{
		ItemId = other.ItemId;
		ItemAmount = other.ItemAmount;
	}
};