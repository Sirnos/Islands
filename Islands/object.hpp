#pragma once

enum class ObjectType : unsigned
{
	UNDEFINED,
	SMALL_STONE,
	STONE,
	BIG_STONE,
	FLINT,
	LEAF_PILE,
	FLOWER,
	VEGETABLE,
	SHRUB,
	TREE,
};

class Object
{
	ObjectType type;
public:
	Object()
	{
		type = ObjectType::UNDEFINED;
	}
	Object(ObjectType Type)
	{
		type = Type;
	}
	ObjectType getType()
	{
		return type;
	}
	void destroy()
	{
		type = ObjectType::UNDEFINED;
	}
};