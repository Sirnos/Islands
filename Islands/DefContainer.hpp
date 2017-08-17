#pragma once

#include "Recipe.hpp"
#include "Structure.hpp"
#include "ItemDef.hpp"
#include "ObjectDef.hpp"
#include <vector>

template<typename DefType>
class DefContainer
{
	std::vector<DefType> Container;

public:
	DefContainer(){}
	~DefContainer()
	{
		for (size_t i = 0; i < Container.size(); i++)
		{
			delete Container[i];
		}
		Container.clear();
	}

	size_t getSize() { return Container.size(); }

	DefType getDefinition(unsigned index)
	{
		return Container[index];
	}
	DefType getDefinition(std::string DefName)
	{
		for (size_t i = 0; i < Container.size(); i++)
		{
			if (Container[i]->getName() == DefName) { return Container[i]; }
		}
		return nullptr;
	}

	unsigned getDefIdbyName(std::string DefName)
	{
		for (size_t i = 0; i < Container.size(); i++)
		{
			if (Container[i]->getName() == DefName.data())
			{
				return i;
			}
		}

		return 0;
	}

	std::vector<DefType> &getContainer() { return Container; }

	void pushNewDef(DefType def)
	{
		Container.push_back(def);
	}
};

typedef DefContainer<ItemDef*> ItemDefContainer;
typedef DefContainer<ObjectDef*> ObjectDefContainer;

namespace makeFromDef
{
	inline std::vector<Recipe> makeRecipe(std::vector<RecipeDef> &def,ItemDefContainer &itemsDef)
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

	inline std::vector<Structure> makeStructure(std::vector<StructureDef> &Def,ObjectDefContainer & ObjsDef)
	{
		std::vector<Structure> Structs;

		for (auto & i : Def)
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
}

