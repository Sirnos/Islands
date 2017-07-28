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
	BaseRecipe(RecType outElement,std::vector<RecType> &inElements)
	{
		Out = outElement;
		In = inElements;
		In.shrink_to_fit();
	}

	RecType getOutElement() { return Out; }
	std::vector<RecType> getIn() { return In; }
	RecType getInElement(size_t index) { return In[index]; }

	size_t getInSize() { return In.size(); }
};

typedef BaseRecipe<RecipeElement> RecipeDef;
typedef BaseRecipe<ItemField> Recipe;
