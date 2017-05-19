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
	virtual ~ItemDef() {}
};

class MeleeWeaponDef : public ItemDef
{
	int Dmg;
	float Reach;
public:
	std::string ifUsed() { return ""; }
	std::string ifClicked()
	{
		return std::string("MATTACK:" + std::to_string(Dmg) + ":" + std::to_string(Reach));
	}
	std::string ifHold() { return "DRAW"; }

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
	std::string ifHold() { return "DRAW"; }
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
	std::string ifUsed() { return "PLACE"; }
	std::string ifHold() { return "DRAW"; }
	std::string ifClicked() { return "MATTACK:1:1"; }

	PlaceableDef(std::string ItemName, unsigned maxStack = 64)
		:ItemDef(ItemName, maxStack) {}
};

class RawMaterialDef : public ItemDef
{
public:
	std::string ifUsed() { return ""; }
	std::string ifHold() { return "DRAW"; }
	std::string ifClicked() { return ""; }

	RawMaterialDef(std::string RMaterialName, unsigned maxStack = 128)
		:ItemDef(RMaterialName, maxStack) {}
};

enum class ArmorPart
{
	Head,
	Chest,
	Legs
};

class ArmorDef : public ItemDef
{
	unsigned protection;
	ArmorPart part;

public:
	std::string ifUsed() { return ""; }
	std::string ifHold() { return ""; }
	std::string ifClicked() { return ""; }

	unsigned getProtectionValue() { return protection; }
	ArmorPart getPart() { return part; }

	ArmorDef(std::string ArmorName, ArmorPart armor,unsigned protectVal, unsigned maxStack = 1)
		:ItemDef(ArmorName, maxStack)
	{
		protection = protectVal;
		part = armor;
	}
};