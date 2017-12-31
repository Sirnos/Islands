#pragma once

const unsigned PLAYER_INVENTORY_SIZE = 5u;
const unsigned PLAYER_ARMOR_INVENTORY_SIZE = 3u;

struct ItemField
{
	unsigned Id;
	unsigned Amount;

	ItemField()
		:Id(0), Amount(0)
	{}
	ItemField(unsigned ItemId, unsigned ItemAmount)
		:Id(ItemId), Amount(ItemAmount)
	{}
	ItemField(const ItemField &other)
		:Id(other.Id), Amount(other.Amount)
	{}
	~ItemField() = default;


	ItemField &operator= (const ItemField &other)
	{
		Id = other.Id;
		Amount = other.Amount;

		return *this;
	}


	ItemField &operator+=(unsigned amount)
	{
		Amount += amount;
		return *this;
	}
	ItemField &operator+=(const ItemField &other)
	{
		if (this->operator==(other))
		{
			Amount += other.Amount;
		}

		return *this;
	}
	ItemField &operator-=(unsigned amount)
	{
		Amount -= amount;
		return *this;
	}
	ItemField &operator-=(const ItemField &other)
	{
		if (this->operator==(other))
		{
			Amount -= other.Amount;
		}
	}


	bool operator==(const ItemField &other) const
	{
		if (Id == other.Id) 
		{ 
			return true; 
		}

		return false;
	}
	bool operator!=(const ItemField &other) const
	{
		return !(operator==(other));
	}


	bool isEmpty() const
	{
		if (Id == 0 || Amount == 0)
		{
			return true;
		}
		return false;
	}
	bool isCorrect() const
	{
		if ((Amount == 0 || Amount == -1) && Id != 0)
		{
			return false;
		}

		return true;
	}
	void empty()
	{
		Id = 0;
		Amount = 0;
	}
};