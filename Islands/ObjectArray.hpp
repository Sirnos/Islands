#pragma once

#include <rapidxml_utils.hpp>
#include <rapidxml_iterators.hpp>
#include <rapidxml.hpp>

#include <vector>
#include <string>
#include <fstream>

#include "object.hpp"
#include "Log.hpp"

class ObjectArray
{
	std::string ObjectsGraphicsFile;
	std::vector<Object> ObjectVector;
public:
	ObjectArray();
	~ObjectArray();

	void loadObjects();

	bool isObjectExist(std::string objectID);
	Object getObject(std::string objectID);

	std::string getObjectsGraphicsFile();
};