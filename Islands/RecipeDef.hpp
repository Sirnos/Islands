#pragma once

#include <utility>
#include <string>
#include <vector>

typedef std::vector<RecipeDef> RecipeVector;
typedef std::pair<std::string, unsigned> RecipeElement;

class RecipeDef
{
	RecipeElement Out;

	size_t InElementSize;
	RecipeElement* In;

	RecipeDef(RecipeElement outElement, size_t inElementsAmount, RecipeElement *inElements)
	{
		Out = outElement;
		In = new RecipeElement[inElementsAmount];
		for (size_t i = 0; i < inElementsAmount; i++)
		{
			In[i] = inElements[i];
		}
	}
	~RecipeDef()
	{
		delete[] In;
	}

	RecipeElement getOutElement()
	{
		return Out;
	}
	size_t getInElementSize()
	{
		return InElementSize;
	}
	RecipeElement getInElement(size_t index)
	{
		return In[index];
	}
};