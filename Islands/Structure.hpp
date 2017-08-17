#pragma once

#include <vector>
#include <string>

typedef std::vector<std::vector<unsigned>> StructureData;
typedef std::pair<unsigned, std::string> StructureInfoKey;
typedef std::vector<StructureInfoKey> StructureInfo;

class StructureBase
{
protected:
	std::string Name;
	unsigned SpawnChance;

public:
	StructureBase(std::string Name, unsigned SpawnChance)
	{
		this->Name = Name;
		this->SpawnChance = SpawnChance;
	}

	std::string getName()
	{
		return Name;
	}
	unsigned getSpawnChance()
	{
		return SpawnChance;
	}
};

class StructureDef : public StructureBase
{
	StructureInfo Info;
	StructureData Data;

public:
	StructureDef(std::string Name, unsigned SpawnChance, StructureData &Data,StructureInfo &Info)
		:StructureBase(Name,SpawnChance)
	{
		this->Data = Data;
		this->Info = Info;
	}

	const StructureData &getData()
	{
		return Data;
	}
	const StructureInfo &getInfo()
	{
		return Info;
	}
};

class Structure : public StructureBase
{
	StructureData Data;

public:
	Structure(std::string Name, unsigned SpawnChance, StructureData &Data)
		:StructureBase(Name, SpawnChance)
	{
		this->Data = Data;
	}
};
