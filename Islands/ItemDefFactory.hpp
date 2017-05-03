#pragma once

#include "ItemDef.hpp"

class ItemDefFactory
{
public:
	virtual ItemDef* createItem() = 0;
};

class MeleeWeaponDefFactory : public ItemDefFactory
{
public:
	MeleeWeaponDef* createItem(std::string Name,int Dmg,float Reach)
	{
		return new MeleeWeaponDef(Name, Dmg, Reach);
	}
};

class DistantWeaponDefFactory : public ItemDefFactory
{
public:
	DistantWeaponDef* create(std::string Name, int Dmg, float Range)
	{
		return new DistantWeaponDef(Name, Dmg, Range);
	}
};

class PlaceableDefFactory : public ItemDefFactory
{
public:
	PlaceableDef* create(std::string Name)
	{
		return new PlaceableDef(Name);
	}
};