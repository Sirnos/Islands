#pragma once

#include <vector>

#include <rapidxml_utils.hpp>
#include <rapidxml_iterators.hpp>
#include <rapidxml.hpp>

#include "DefContainer.hpp"

class GameComponentsLoader
{
public:
	void LoadObjectDefFromFile(std::vector<ObjectDef*> &Objects,std::string &ObjectsGraphicsFile,std::vector<sf::IntRect> &Textures)
	{
		Objects.push_back(new ObjectDef());
		Textures.push_back(sf::IntRect());

		rapidxml::file<char> File("Data/Objects.xml");
		rapidxml::xml_document<> document;
		document.parse<0>(File.data());

		for (rapidxml::xml_node<>* node1 = document.first_node(); node1 != nullptr;
			node1 = node1->next_sibling())
		{
			ObjectsGraphicsFile = node1->first_attribute()->value();
				for (rapidxml::xml_node<>* node2 = node1->first_node(); node2 != nullptr;
					node2 = node2->next_sibling())
				{
					rapidxml::xml_node<>* node3 = node2->first_node();

					std::string ObjName = node2->first_attribute()->value();
					bool isDestructible = false;
					Yield IFDESTROYED;
					sf::Vector2f size;
					sf::IntRect textureCords;
					sf::FloatRect collisionBox;

					for (node3; node3 != nullptr; node3 = node3->next_sibling())
					{
						std::string parametrName(node3->name());
						rapidxml::xml_node<>* node4 = node3->first_node();
						if (parametrName == "ISDESTRUCTIBLE")
						{
							if (node4->value() == "1")
							{
								isDestructible = true;
							}
						}

						for (node4; node4 != nullptr; node4 = node4->next_sibling())
						{
							std::string nodeName = node4->name();
							if (nodeName == "DROP")
							{
								IFDESTROYED.first = std::string(node4->value());
							}
							else if (nodeName == "AMOUNT")
							{
								IFDESTROYED.second
									= static_cast<unsigned>(std::stoi(std::string(node4->value())));
							}
							else if (nodeName == "X")
							{
								size.x = static_cast<float>(std::stoi(std::string(node4->value())));
							}
							else if (nodeName == "Y")
							{
								size.y = static_cast<float>(std::stoi(std::string(node4->value())));
							}
							else if (nodeName == "TOP")
							{
								if (parametrName == "COLLISION")
									collisionBox.top
									= static_cast<float>(std::stoi(std::string(node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.top = std::stoi(std::string(node4->value()));
							}
							else if (nodeName == "LEFT")
							{
								if (parametrName == "COLLISION")
									collisionBox.left
									= static_cast<float>(std::stoi(std::string(node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.left = std::stoi(std::string(node4->value()));
							}
							else if (nodeName == "WIDTH")
							{
								if (parametrName == "COLLISION")
									collisionBox.width
									= static_cast<float>(std::stoi(std::string(node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.width = std::stoi(std::string(node4->value()));
							}
							else if (nodeName == "HEIGHT")
							{
								if (parametrName == "COLLISION")
									collisionBox.height
									= static_cast<float>(std::stoi(std::string(node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.height = std::stoi(std::string(node4->value()));
							}
						}
					}
					Objects.push_back(new ObjectDef(ObjName, size, collisionBox, IFDESTROYED, isDestructible));
					Textures.push_back(textureCords);
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

		int textureSize;

		for (auto node1 = document.first_node(); node1 != nullptr; node1 = node1->next_sibling())
		{
			std::string node1Name = node1->name();
			if (node1Name == "Items")
			{
				auto attrib = node1->first_attribute();

				std::string attribName = attrib->name();
				if (attribName == "graphics")
				{
					ItemsGraphicsFile = attrib->value();
					attrib = attrib->next_attribute();
					attribName = attrib->name();
					if (attribName == "textureSize")
					{
						auto temp = std::stoi(attrib->value());
						if (temp > 64) { temp = 64; }
						textureSize = temp;
					}
				}
			}
			for (auto node2 = node1->first_node(); node2 != nullptr; node2 = node2->next_sibling())
			{
				std::string node2Name = node2->name();
				if (node2Name == "Item")
				{
					std::string name;
					std::string type;
					unsigned maxStack;
					sf::Vector2i texturePos;

					name = node2->first_attribute()->value();

					for (auto node3 = node2->first_node(); node3 != nullptr; node3 = node3->next_sibling())
					{
						std::string node3Name = node3->name();
						if (node3Name == "GRAPHICS")
						{
							for (auto node4 = node3->first_node(); node4 != nullptr; node4 = node4->next_sibling())
							{

								auto tempPos = std::stoi(node4->value());
								std::string node4Name = node4->name();

								if (node4Name == "POSX")
								{
									texturePos.x = tempPos;
								}
								else if (node4Name == "POSY")
								{
									texturePos.y = tempPos;
								}
							}
						}
						else if(node3Name == "MAXSTACK")
						{
							maxStack = static_cast<unsigned>(std::stoi(node3->value()));
						}
						else if(node3Name == "TYPE")
						{
							type = node3->value();
						}
					}

					if (type == "Material")
					{
						Items.push_back(new RawMaterialDef(name, maxStack));
						textures.push_back(sf::IntRect(texturePos.x * textureSize, texturePos.y * textureSize, textureSize, textureSize));
					}
				}
			}
		}
	}
};