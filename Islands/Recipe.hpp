#pragma once

#include "ItemField.hpp"
#include <utility>
#include <string>
#include <vector>

typedef std::pair<std::string, unsigned> RecipeElement;

template<typename RecType>
class BaseRecipe
{
	RecType Out;
	std::vector<RecType> In;
public:
	BaseRecipe(const RecType &outElement, const std::vector<RecType> &inElements)
		:Out(outElement), In(inElements)
	{
		In.shrink_to_fit();
	}
	~BaseRecipe() = default;

	RecType getOutElement() const { return Out; }
	std::vector<RecType> getIn() const { return In; }
	RecType getInElement(size_t index) const { return In[index]; }

	size_t getInSize() const { return In.size(); }
};

typedef BaseRecipe<RecipeElement> RecipeDef;
typedef BaseRecipe<ItemField> Recipe;
