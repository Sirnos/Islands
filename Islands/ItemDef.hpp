#pragma once

#include <string>

const unsigned MAXIMUM_STACK_SIZE = 255;

enum class ItemType
{
	Default,
	Placeable,
	MeleeWeapon,
	DistanceWeapon,
	Armor
};

inline std::string ItemTypeToString(ItemType type)
{
	std::string ret("ItemType::");

	switch (type)
	{
	case ItemType::Default:
		ret += "Default";
		break;
	case ItemType::Placeable:
		ret += "Placeable";
		break;
	case ItemType::MeleeWeapon:
		ret += "MeleeWeapon";
		break;
	case ItemType::DistanceWeapon:
		ret += "DistanceWeapon";
		break;
	case ItemType::Armor:
		ret += "Armor";
		break;
	default:
		ret += "!IsUndefided";
		break;
	}

	return ret;
}

class ItemDef
{
	std::string name;
	unsigned maxStack;
	ItemType type;
public:
	virtual	std::string ifUsed() = 0;
	virtual	std::string ifClicked() = 0;
	virtual std::string ifHold() = 0;

	std::string getName() { return name; }
	unsigned getMaxStack() { return maxStack; }

	ItemType getType() { return type; }

	ItemDef(std::string &ItemName, unsigned maxItemStack,ItemType typeOfItem = ItemType::Default)
		:name(ItemName), maxStack(maxItemStack), type(typeOfItem)
	{
		if (maxItemStack < MAXIMUM_STACK_SIZE)
			maxStack = maxItemStack;
		else
			maxStack = MAXIMUM_STACK_SIZE;
	}
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

	MeleeWeaponDef(std::string &ItemName, int WeaponDmg, float WeaponReach, unsigned maxStack = 1)
		:ItemDef(ItemName, maxStack, ItemType::MeleeWeapon), Dmg(WeaponDmg), Reach(WeaponReach){}
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
	DistantWeaponDef(std::string &ItemName, int WeaponDmg, float WeaponRange, unsigned maxStack = 1)
		:ItemDef(ItemName, maxStack, ItemType::DistanceWeapon), Dmg(WeaponDmg), Range(WeaponRange){}
};

class PlaceableDef : public ItemDef
{
public:
	std::string ifUsed() { return "PLACE"; }
	std::string ifHold() { return "DRAW"; }
	std::string ifClicked() { return "MATTACK:1:1"; }

	PlaceableDef(std::string &ItemName, unsigned maxStack = 64)
		:ItemDef(ItemName, maxStack, ItemType::Placeable) {}
};

class RawMaterialDef : public ItemDef
{
public:
	std::string ifUsed() { return ""; }
	std::string ifHold() { return "DRAW"; }
	std::string ifClicked() { return ""; }

	RawMaterialDef(std::string &RMaterialName, unsigned maxStack = 128)
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

	ArmorDef(std::string &ArmorName, ArmorPart armor,unsigned protectVal, unsigned maxStack = 1)
		:ItemDef(ArmorName, maxStack, ItemType::Armor), protection(protectVal), part(armor){}
};