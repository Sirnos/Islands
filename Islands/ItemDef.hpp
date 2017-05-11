#pragma once

#include <string>

class ItemDef
{
	std::string name;
	unsigned maxStack;

public:
	virtual	std::string ifUsed() = 0;
	virtual	std::string ifClicked() = 0;
	virtual std::string ifHold() = 0;

	std::string getName() { return name; }
	unsigned getMaxStack() { return maxStack; }

	ItemDef(std::string ItemName, unsigned maxItemStack) { name = ItemName; maxStack = maxItemStack; }
	virtual ~ItemDef(){}
};

class MeleeWeaponDef :public ItemDef
{
	int Dmg;
	float Reach;
public:
	std::string ifUsed() { return ""; }
	std::string ifClicked()
	{
		return std::string("MATTACK:" + std::to_string(Dmg) + ":" + std::to_string(Reach));
	}
	std::string ifHold()
	{
		return std::string("DRAW");
	}

	MeleeWeaponDef(std::string ItemName, int WeaponDmg, float WeaponReach, unsigned maxStack = 1)
		:ItemDef(ItemName, maxStack)
	{
		Dmg = WeaponDmg;
		Reach = WeaponReach;
	}
};

class DistantWeaponDef :public ItemDef
{
	int Dmg;
	float Range;

public:
	std::string ifUsed() { return ""; }
	std::string ifClicked()
	{
		return std::string("DATTACK:" + std::to_string(Dmg) + ":" + std::to_string(Range));
	}
	std::string ifHold()
	{
		return std::string("DRAW");
	}
	DistantWeaponDef(std::string ItemName, int WeaponDmg, float WeaponRange, unsigned maxStack = 1)
		:ItemDef(ItemName, maxStack)
	{
		Dmg = WeaponDmg;
		Range = WeaponRange;
	}
};

class PlaceableDef : public ItemDef
{
public:
	std::string ifUsed()
	{
		return std::string("PLACE");
	}
	std::string ifHold()
	{
		return std::string("DRAW");
	}
	std::string ifClicked()
	{
		return std::string("MATTACK:1:1");
	}

	PlaceableDef(std::string ItemName, unsigned maxItemStack = 64)
		:ItemDef(ItemName, maxItemStack) {}
};