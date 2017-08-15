#pragma once

#include <vector>
#include <string>

typedef std::vector<std::vector<unsigned>> StructureData;
typedef std::pair<unsigned, std::string> StructureInfoKey;
typedef std::vector<StructureInfoKey> StructureInfo;

class Structure
{
	std::string Name;
	unsigned SpawnChance;

	StructureInfo Info;
	StructureData Data;

public:
	Structure(std::string Name, unsigned SpawnChance, StructureData &Data,StructureInfo &Info)
	{
		this->Name = Name;
		this->SpawnChance = SpawnChance;
		this->Data = Data;
		this->Info = Info;
	}

	const StructureData &getData()
	{
		return Data;
	}
	std::string getName()
	{
		return Name;
	}
	unsigned getSpawnChance() 
	{
		return SpawnChance;
	}
	const StructureInfo &getInfo()
	{
		return Info;
	}
};