#pragma once

#include "Recipe.hpp"
#include "Structure.hpp"
#include "BiomeValues.hpp"
#include "ItemDef.hpp"
#include "ObjectDef.hpp"
#include "EntityDef.hpp"
#include <vector>
#include <map>

template<typename DefType, typename = typename std::enable_if<std::is_pointer<DefType>::value, DefType>::type>
class DynamicDefContainer
{
	std::vector<DefType> Container;
	std::map<std::string, size_t> ContainerMap;
public:
	DynamicDefContainer()
		:Container(1)
	{
		ContainerMap.insert(std::pair<std::string, size_t>("", 0));
	}
	DynamicDefContainer(const DynamicDefContainer & other) = delete;
	~DynamicDefContainer()
	{
		for (const auto & elem : Container)
		{
			delete elem;
		}
	}

	size_t getSize() const { return Container.size(); }

	const DefType getDefinition(unsigned index) const
	{
		return Container[index];
	}
	const DefType getDefinition(const std::string &DefName) const
	{
		auto index = ContainerMap.find(DefName);
		if (index != ContainerMap.end())
		{
			return Container[index->second];
		}
		return nullptr;
	}

	size_t getDefIdbyName(const std::string &DefName) const
	{
		auto index = ContainerMap.find(DefName);
		if (index != ContainerMap.end())
		{
			return index->second;
		}
		return 0;
	}

	const std::vector<DefType> &getContainer() const { return Container; }
	std::vector<DefType> &getContainer(){ return Container; }

	void pushNewDef(DefType def)
	{
		if (def != nullptr)
		{
			Container.push_back(def);
			ContainerMap.insert(std::pair<std::string, size_t>(def->getName(), Container.size() - 1));
		}
	}
};

template<typename DefType>
class StaticDefContainer
{
	std::vector<DefType> Container;
public:
	StaticDefContainer() = default;
	StaticDefContainer(const StaticDefContainer &other) = delete;
	~StaticDefContainer() = default;

	size_t getSize() const { return Container.size(); }

	const DefType &getDefinition(unsigned index) const
	{
		return Container[index];
	}
	const DefType &getDefinition(const std::string &defName) const
	{
		for (const auto &elem : Container)
		{
			if (elem.getName() == defName)
			{
				return elem;
			}
		}
		return Container.front();
	}

	unsigned getDefIdbyName(const std::string &defName) const
	{
		for (size_t i = 0; i < getSize(); i++)
		{
			if (Container[i].getName() == defName)
			{
				return i;
			}
		}
		return 0;
	}

	std::vector<DefType> &getContainer() { return Container; }

	void pushNewDef(const DefType &def)
	{
		Container.push_back(def);
	}
};

typedef DynamicDefContainer<ItemDef*> ItemDefContainer;
typedef DynamicDefContainer<ObjectDef*> ObjectDefContainer;
typedef StaticDefContainer <EntityDef> EntityDefContainer;
typedef StaticDefContainer <MonsterEntityDef> MonsterDefContainer;

namespace makeFromDef
{
	inline std::vector<Recipe> makeRecipe(const std::vector<RecipeDef> &def, const ItemDefContainer &itemsDef)
	{
		std::vector<Recipe> out;

		std::vector<ItemField> recipeInElement;
		for (size_t i = 0; i < def.size(); i++)
		{
			ItemField recipeOutElement;

			recipeOutElement.ItemId = itemsDef.getDefIdbyName(def[i].getOutElement().first);
			recipeOutElement.ItemAmount = def[i].getOutElement().second;

			for (size_t j = 0; j < def[i].getInSize(); j++)
			{
				recipeInElement.push_back(ItemField(itemsDef.getDefIdbyName(def[i].getInElement(j).first),
					def[i].getInElement(j).second));
				if (recipeInElement.back().isEmpty()) { recipeInElement.pop_back(); }
			}
			out.push_back(Recipe(recipeOutElement, recipeInElement));

			if (out.back().getOutElement().isEmpty() && out.size() > 0)
			{
				out.pop_back();
			}
			recipeInElement.clear();
		}
		return out;
	}

	inline std::vector<Structure> makeStructure(const std::vector<StructureDef> &Def, const ObjectDefContainer & ObjsDef)
	{
		std::vector<Structure> Structs;

		for (const auto & i : Def)
		{
			std::vector<unsigned> Ids;
			StructureData Data;

			for (auto & DefInfo : i.getInfo())
			{
				Ids.push_back(ObjsDef.getDefIdbyName(DefInfo.second));
			}

			for (auto & DataRow : i.getData())
			{
				Data.push_back(std::vector<unsigned>());
				for (auto & DataCell : DataRow)
				{
					if (DataCell == 0)
					{
						Data.back().push_back(0);
					}
					else
					{
						for (size_t infoIndex = 0; infoIndex < i.getInfo().size(); infoIndex++)
						{
							if (DataCell == i.getInfo()[infoIndex].first)
							{
								Data.back().push_back(Ids[infoIndex]);
								break;
							}
						}
					}
				}
			}

			Structs.push_back(Structure(i.getName(), i.getSpawnChance(), Data));
		}
		return Structs;
	}

	inline std::vector<BiomeValues> makeBiome(std::vector<BiomeValuesDef> & Defs, const ObjectDefContainer & ObjsDef, const std::vector<Structure> & Structs)
	{
		std::vector<BiomeValues> ret;

		for (auto & biome : Defs)
		{
			ret.push_back(BiomeValues());
			ret.back().BiomeName = biome.BiomeName;

			for (auto & biomeTerrain : biome.TerrainTiles)
			{
				ret.back().TerrainTiles.push_back(biomeTerrain);
			}

			for (auto & biomeObjects : biome.SpawnableObjects)
			{
				size_t objectId = 0;
				objectId = ObjsDef.getDefIdbyName(std::get<0>(biomeObjects));

				if (objectId != 0)
				{
					ret.back().SpawnableObjects.push_back(std::tuple<size_t, float, TerrainType>
						(objectId, std::get<1>(biomeObjects), std::get<2>(biomeObjects)));
				}
			}
			for (auto & biomeStructures : biome.SpawnableStructures)
			{
				size_t structId = -1;
				unsigned structChance = biomeStructures.second;

				for (size_t i = 0; i < Structs.size(); i++)
				{
					if (biomeStructures.first == Structs[i].getName())
					{
						structId = i;
						break;
					}
				}

				if (structId != -1)
				{
					ret.back().SpawnableStructures.push_back(std::pair<size_t, unsigned>(structId, structChance));
				}
			}
		}
		return ret;
	}
}
