#pragma once

#include <vector>

#include <rapidxml_utils.hpp>
#include <rapidxml_iterators.hpp>
#include <rapidxml.hpp>

#include "DefContainer.hpp"

class GameComponentsLoader
{
	sf::Vector2i getVectorFromString(std::string str)
	{
		sf::Vector2i temp;
		temp.x = std::stoi(str.substr(0, str.find(',')));
		temp.y = std::stoi(str.substr(str.find(',') + 1, str.size() - 1));
		return temp;
	}
	sf::IntRect getIntRectFromString(std::string str)
	{
		short varNumber = 0;
		std::string values[4];

		for (auto i : str)
		{
			if (i == ',')
			{
				if (varNumber == 3) { break; }
				varNumber++;
			}
			else
			{
				values[varNumber] += i;
			}
		}

		return sf::IntRect(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]), std::stoi(values[3]));
	}
	sf::FloatRect getFloatRectFromString(std::string str)
	{
		short varNumber = 0;
		std::string values[4];

		for (auto i : str)
		{
			if (i == ',')
			{
				if (varNumber == 3) { break; }
				varNumber++;
			}
			else
			{
				values[varNumber] += i;
			}
		}

		return sf::FloatRect(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]), std::stof(values[3]));
	}
	Yield getYieldFromString(std::string str)
	{
		return Yield(str.substr(0, str.find(',')), std::stoul(str.substr(str.find(',') + 1, str.size() - 1)));
	}

public:
	void LoadObjectDefFromFile(std::vector<ObjectDef*> &Objects,std::string &ObjectsGraphicsFile,std::vector<sf::IntRect> &Textures)
	{
		Objects.push_back(new ObjectDef());
		Textures.push_back(sf::IntRect());

		rapidxml::file<char> File("Data/Objects.xml");
		rapidxml::xml_document<> document;
		document.parse<0>(File.data());

		for (rapidxml::xml_node<>* mainNode = document.first_node(); mainNode != nullptr;
			mainNode = mainNode->next_sibling())
		{
			std::vector<unsigned> tempUint;
			std::vector<float> tempFloat;
			std::vector<std::string> tempString;

			ObjectsGraphicsFile = mainNode->first_attribute()->value();
			for (rapidxml::xml_node<>* objectsNode = mainNode->first_node(); objectsNode != nullptr;
				objectsNode = objectsNode->next_sibling())
			{
				std::string newObjName = objectsNode->first_attribute()->value();
				bool newObjDestructible = false;
				Yield newObjYield;
				sf::Vector2i newObjSize;
				sf::IntRect newObjTextureCoord;
				sf::FloatRect newObjCollisionBox;

				ObjectType newObjType = ObjectType::Default;


				for (rapidxml::xml_node<>* objectsParamNode = objectsNode->first_node(); objectsParamNode != nullptr;
					objectsParamNode = objectsParamNode->next_sibling())
				{
					std::string paramName = objectsParamNode->name();

					if (paramName == "Size")
					{
						newObjSize = getVectorFromString(std::string(objectsParamNode->value()));
					}
					else if(paramName == "Graphics")
					{
						newObjTextureCoord = getIntRectFromString(std::string(objectsParamNode->value()));
					}
					else if(paramName == "Collision")
					{
						newObjCollisionBox = getFloatRectFromString(std::string(objectsParamNode->value()));
					}
					else if(paramName == "Isdestructible")
					{
						std::string temp = objectsParamNode->value();
						if (temp == "1") { newObjDestructible = true; }
					}
					else if(paramName == "Ifdestroyed")
					{
						newObjYield = getYieldFromString(std::string(objectsParamNode->value()));
					}
					else if(paramName == "ChestDef")
					{
						newObjType = ObjectType::Chest;
						tempUint.push_back(std::stoul(std::string(objectsParamNode->first_node()->value())));
					}
					else if(paramName == "SaplingDef")
					{
						newObjType = ObjectType::Sapling;
						tempFloat.push_back(std::stof(std::string(objectsParamNode->first_node()->value())));
						tempString.push_back(std::string(objectsParamNode->first_node()->next_sibling()->value()));
					}
				}

				switch (newObjType)
				{
				case ObjectType::Default:
					Objects.push_back(new ObjectDef(newObjName, newObjSize, newObjCollisionBox, newObjYield, newObjDestructible));
					break;
				case ObjectType::Chest:
					Objects.push_back(new ChestDef(newObjName, newObjSize, newObjCollisionBox, newObjYield, newObjDestructible, tempUint.back()));
					break;
				case ObjectType::Tree:
					break;
				case ObjectType::Sapling:
					Objects.push_back(new SaplingDef(newObjName, newObjSize, newObjCollisionBox, newObjYield, newObjDestructible, tempFloat.back(), tempString.back()));
					break;
				case ObjectType::Spawner:
					break;
				default:
					break;
				}
				Textures.push_back(newObjTextureCoord);
				tempFloat.clear();
				tempString.clear();
				tempUint.clear();
			}
		}
		Objects.shrink_to_fit();
	}

	void GenerateItemsFromObjectDef(std::vector<ObjectDef*> &Objs,std::vector<ItemDef*> &Items)
	{
		for (unsigned i = 1; i < Objs.size(); i++)
		{
			Items.push_back(new PlaceableDef(Objs[i]->getName()));
			if (Items.back() == nullptr)
			{
				Items.pop_back();
			}
		}
		Items.shrink_to_fit();
	}

	void LoadItemDefFromFile(std::vector<ItemDef*> &Items,std::string &ItemsGraphicsFile,std::vector<sf::IntRect> &textures)
	{
		rapidxml::file<char> File("Data/Items.xml");
		rapidxml::xml_document<> document;
		document.parse<0>(File.data());

		for (rapidxml::xml_node<>* mainNode = document.first_node(); mainNode != nullptr;
			mainNode = mainNode->next_sibling())
		{
			unsigned textureSize;

			rapidxml::xml_attribute<>* mainAttrib;
			mainAttrib = mainNode->first_attribute();
			ItemsGraphicsFile = mainAttrib->value();
			mainAttrib = mainAttrib->next_attribute();
			textureSize = std::stoul(std::string(mainAttrib->value()));

			for (rapidxml::xml_node<>* itemsNode = mainNode->first_node(); itemsNode != nullptr;
				itemsNode = itemsNode->next_sibling())
			{
				std::string newItemName;
				unsigned newItemMaxStack;

				sf::Vector2i texturePos;

				sf::IntRect newItemTextureCoord;

				newItemName = itemsNode->first_attribute()->value();

				for (rapidxml::xml_node<>* itemParamNode = itemsNode->first_node(); itemParamNode != nullptr;
					itemParamNode = itemParamNode->next_sibling())
				{
					std::string paramName = itemParamNode->name();

					if (paramName == "Graphics")
					{
						texturePos = getVectorFromString(std::string(itemParamNode->value()));
					}
					else if(paramName == "MaxStack")
					{
						newItemMaxStack = std::stoul(std::string(itemParamNode->value()));
					}
				}
				Items.push_back(new RawMaterialDef(newItemName, newItemMaxStack));
				textures.push_back(sf::IntRect(texturePos.x * textureSize, texturePos.y * textureSize, textureSize, textureSize));
			}
			Items.shrink_to_fit();
		}
	}
};