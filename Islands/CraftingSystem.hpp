#pragma once

#include "Recipe.hpp"
#include "DefContainer.hpp"
#include "playerCharacter.hpp"

class CraftingSystem
{
	std::vector<Recipe> AvailableRecipes;

	unsigned SelectedRecipe;
	unsigned CraftAmount;
public:
	CraftingSystem(){}
	~CraftingSystem(){}

	void PopRecipes()
	{
		AvailableRecipes.pop_back();
		clearPlayerSelects();
	}
	void AddNewRecipes(std::vector<Recipe> & recs)
	{
		for (auto & i : recs) { AvailableRecipes.push_back(i); }
		clearPlayerSelects();
	}

	void clearPlayerSelects()
	{
		SelectedRecipe = 0;
		CraftAmount = 0;
	}
	void clear()
	{
		AvailableRecipes.clear();
		clearPlayerSelects();
	}

	void setSelectedRecipe(unsigned newSelect) { SelectedRecipe = newSelect; }
	unsigned getSelectedRecipeNumber() { return SelectedRecipe; }

	ItemField getRecipeOut(unsigned Index)
	{
		return AvailableRecipes[Index].getOutElement();
	}
	unsigned getRecipesNumber() { return AvailableRecipes.size(); }

	void setCraftAmount(unsigned newAmount) { CraftAmount = newAmount; }
	unsigned getCraftAmount() { return CraftAmount; }

	const std::vector<Recipe> & getRecipes()
	{
		return AvailableRecipes;
	}

	ItemField craftItemFromRecipe(playerCharacter &player,ItemDefContainer &ItemsDef)
	{
		if (SelectedRecipe >= AvailableRecipes.size()) { return ItemField(); }

		unsigned craftedItemId = AvailableRecipes[SelectedRecipe].getOutElement().ItemId;
		unsigned craftedItemAmount = CraftAmount * AvailableRecipes[SelectedRecipe].getOutElement().ItemAmount;
		std::vector<ItemField> neededResources = AvailableRecipes[SelectedRecipe].getIn();
		std::vector<ItemField> availableResources;
		std::vector<ItemField> restResources;
		availableResources.resize(neededResources.size(), ItemField());

		sf::Vector2u inventoryIndex = sf::Vector2u(0, 0);

		for (inventoryIndex.x; inventoryIndex.x < PlayerInventorySize; inventoryIndex.x++)
		{
			inventoryIndex.y = 0;
			for (inventoryIndex.y; inventoryIndex.y < PlayerInventorySize; inventoryIndex.y++)
			{
				ItemField inventoryItem = player.getInventoryField(inventoryIndex);
				if (inventoryItem.ItemId != 0)
				{
					for (size_t i = 0; i < neededResources.size(); i++)
					{
						if (neededResources[i].ItemId == inventoryItem.ItemId)
						{
							availableResources[i] += inventoryItem.ItemAmount;
							player.setInventoryField(inventoryIndex, ItemField());
							break;
						}
					}
				}
			}
		}

		unsigned maxAmount = CraftAmount;
		for (size_t i = 0; i < availableResources.size(); i++)
		{
			if (availableResources[i].ItemAmount < neededResources[i].ItemAmount)
			{
				maxAmount = 0;
				break;
			}
			else
			{
				unsigned craftAmountPerItem = availableResources[i].ItemAmount / neededResources[i].ItemAmount;
				if (craftAmountPerItem < CraftAmount) { CraftAmount = craftAmountPerItem; }
			}
		}

		ItemField craftedItem;
		if (maxAmount > 0)
		{
			for (size_t i = 0; i < neededResources.size(); i++)
			{
				restResources.push_back(ItemField(neededResources[i].ItemId,
					availableResources[i].ItemAmount - (maxAmount * neededResources[i].ItemAmount)));
				if (restResources.back().ItemAmount == -1 || restResources.back().ItemAmount == 0)
				{
					restResources.pop_back();
				}
			}

			craftedItem = ItemField(craftedItemId, maxAmount * AvailableRecipes[SelectedRecipe].getOutElement().ItemAmount);
		}

		inventoryIndex = sf::Vector2u(0, 0);
		for (inventoryIndex.x; inventoryIndex.x < PlayerInventorySize; inventoryIndex.x++)
		{
			inventoryIndex.y = 0;
			for (inventoryIndex.y; inventoryIndex.y < PlayerInventorySize; inventoryIndex.y++)
			{
				if (restResources.size() == 0) { return craftedItem; }
				if (player.getInventoryField(inventoryIndex).isEmpty())
				{
					player.setInventoryField(inventoryIndex, restResources.back());
					restResources.pop_back();
				}
			}
		}
		return craftedItem;
	}
};