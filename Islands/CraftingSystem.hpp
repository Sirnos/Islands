#pragma once

#include "Recipe.hpp"
#include "DefContainer.hpp"
#include "PlayerInventory.hpp"

class CraftingSystem
{
	std::shared_ptr<ItemDefContainer> ItemsDef;

	std::vector<Recipe> PlayerBaseRecipes;
	std::vector<Recipe> AvailableRecipes;

	unsigned SelectedRecipe;
	unsigned CraftAmount;
public:
	CraftingSystem()
		:CraftAmount(1),SelectedRecipe(0)
	{}
	CraftingSystem(const CraftingSystem& other) = delete;
	~CraftingSystem() = default;

	void AssingItemDef(const std::shared_ptr<ItemDefContainer> &ItemsDef)
	{
		this->ItemsDef = ItemsDef;
	}

	void PopRecipes()
	{
		AvailableRecipes.pop_back();
		clearPlayerSelects();
	}
	void AddNewRecipes(const std::vector<Recipe> & recs)
	{
		for (auto & i : recs) { AvailableRecipes.push_back(i); }
		clearPlayerSelects();
	}
	void AssingRecipes(const std::vector<Recipe> & recs)
	{
		AvailableRecipes = recs;
	}

	void clearPlayerSelects()
	{
		SelectedRecipe = 0;
		CraftAmount = 1;
	}
	void clear()
	{
		AvailableRecipes.clear();
		clearPlayerSelects();
	}

	void loadPlayerRecipes(const std::vector<Recipe> &DefaultRecipes)
	{
		PlayerBaseRecipes = DefaultRecipes;
	}
	void usePlayerRecipes() { AvailableRecipes = PlayerBaseRecipes; }
	bool isUsedPlayerRecipes() const
	{
		if (PlayerBaseRecipes[0].getOutElement().ItemId == AvailableRecipes[0].getOutElement().ItemId)
		{
			return true;
		}

		return false;
	}

	void setSelectedRecipe(unsigned newSelect) { SelectedRecipe = newSelect; }
	unsigned getSelectedRecipeNumber() const { return SelectedRecipe; }

	ItemField getRecipeOut(unsigned Index) const
	{
		return AvailableRecipes[Index].getOutElement();
	}
	unsigned getRecipesNumber() const
	{
		return AvailableRecipes.size();
	}

	void setCraftAmount(unsigned newAmount) { CraftAmount = newAmount; }
	unsigned getCraftAmount() const { return CraftAmount; }

	const std::vector<Recipe> & getRecipes() const
	{
		return AvailableRecipes;
	}

	ItemField craftItemFromRecipe(PlayerInventory &Inv)
	{
		if (SelectedRecipe >= AvailableRecipes.size()) { return ItemField(); }

		unsigned craftedItemId = AvailableRecipes[SelectedRecipe].getOutElement().ItemId;
		unsigned craftedItemAmount = CraftAmount * AvailableRecipes[SelectedRecipe].getOutElement().ItemAmount;
		std::vector<ItemField> neededResources = AvailableRecipes[SelectedRecipe].getIn();
		std::vector<ItemField> availableResources;
		std::vector<ItemField> restResources;
		availableResources.resize(neededResources.size(), ItemField());

		sf::Vector2u inventoryIndex = sf::Vector2u(0, 0);
		for (inventoryIndex.x = 0; inventoryIndex.x < PLAYER_INVENTORY_SIZE; inventoryIndex.x++)
		{
			for (inventoryIndex.y = 0; inventoryIndex.y < PLAYER_INVENTORY_SIZE; inventoryIndex.y++)
			{
				ItemField inventoryItem = Inv.getInventoryField(inventoryIndex);
				if (inventoryItem.ItemId != 0)
				{
					for (size_t i = 0; i < neededResources.size(); i++)
					{
						if (neededResources[i].ItemId == inventoryItem.ItemId)
						{
							availableResources[i] += inventoryItem.ItemAmount;
							Inv.setInventoryField(inventoryIndex, ItemField());
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

		for (inventoryIndex.x = 0; inventoryIndex.x < PLAYER_INVENTORY_SIZE; inventoryIndex.x++)
		{
			for (inventoryIndex.y = 0; inventoryIndex.y < PLAYER_INVENTORY_SIZE; inventoryIndex.y++)
			{
				if (restResources.size() == 0) { return craftedItem; }
				if (Inv.getInventoryField(inventoryIndex).isEmpty())
				{
					Inv.setInventoryField(inventoryIndex, restResources.back());
					restResources.pop_back();
				}
			}
		}
		return craftedItem;
	}
};
