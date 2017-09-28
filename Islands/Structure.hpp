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
	StructureBase(const std::string &Name, unsigned SpawnChance)
		:Name(Name), SpawnChance(SpawnChance)
	{}

	std::string getName() const
	{
		return Name;
	}
	unsigned getSpawnChance() const
	{
		return SpawnChance;
	}
};

class StructureDef : public StructureBase
{
	StructureInfo Info;
	StructureData Data;

public:
	StructureDef(const std::string &Name, unsigned SpawnChance, const StructureData &Data, const StructureInfo &Info)
		:StructureBase(Name, SpawnChance), Data(Data), Info(Info)
	{}

	const StructureData &getData() const
	{
		return Data;
	}
	const StructureInfo &getInfo() const
	{
		return Info;
	}
};

class Structure : public StructureBase
{
	StructureData Data;

public:
	Structure(const std::string &Name, unsigned SpawnChance, const StructureData &Data)
		:StructureBase(Name, SpawnChance), Data(Data)
	{}

	const StructureData &getData() const
	{
		return Data;
	}
};
