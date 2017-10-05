#pragma once

const unsigned PLAYER_INVENTORY_SIZE = 5u;
const unsigned PLAYER_ARMOR_INVENTORY_SIZE = 3u;

struct ItemField
{
	unsigned ItemId;
	unsigned ItemAmount;

	ItemField()
		:ItemId(0),ItemAmount(0)
	{}
	ItemField(unsigned newItemId, unsigned newItemAmount)
		:ItemId(newItemId),ItemAmount(newItemAmount)
	{}
	ItemField(const ItemField &other)
		:ItemId(other.ItemId),ItemAmount(other.ItemAmount)
	{}
	~ItemField() = default;

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

	bool isEmpty() const
	{
		if (ItemId == 0 || ItemAmount == 0)
		{
			return true;
		}
		return false;
	}
	bool isCorrect() const
	{
		if ((ItemAmount == 0 || ItemAmount == -1) && ItemId != 0)
		{
			return false;
		}
		return true;
	}
	void empty() { *this = ItemField(); }
};