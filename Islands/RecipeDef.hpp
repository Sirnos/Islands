#pragma once

#include <utility>
#include <string>
#include <vector>

typedef std::pair<std::string, unsigned> RecipeElement;
typedef std::vector<RecipeElement> InRec;

class RecipeDef
{
	RecipeElement Out;
	InRec In;
public:
	RecipeDef(RecipeElement outElement,const InRec &inElements)
	{
		Out = outElement;
		In = inElements;
		In.shrink_to_fit();
	}
	~RecipeDef()
	{
		In.clear();
	}

	RecipeElement getOutElement()
	{
		return Out;
	}
	size_t getInElementSize()
	{
		return In.size();
	}
	RecipeElement getInElement(size_t index)
	{
		return In[index];
	}
};

typedef std::vector<RecipeDef> RecipeVector;