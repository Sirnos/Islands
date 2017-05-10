#pragma once

#include <vector>

#include <rapidxml_utils.hpp>
#include <rapidxml_iterators.hpp>
#include <rapidxml.hpp>

#include "ObjectDef.hpp"
#include "ItemDef.hpp"

class GameComponentsLoader
{
public:
	std::vector<ObjectDef> LoadObjectDefFromFile(std::string file,std::string &ObjectsGraphicsFile)
	{
		std::vector<ObjectDef> Objects;
		Objects.push_back(ObjectDef());
		rapidxml::file<> File("Data/Objects.xml");
		rapidxml::xml_document<> document;
		document.parse<0>(File.data());

		for (rapidxml::xml_node<>* node = document.first_node(); node != NULL;
			node = node->next_sibling())
		{
			ObjectsGraphicsFile = node->first_attribute()->value();
			for (rapidxml::xml_node<>* nNode = node->first_node(); nNode != NULL;
				nNode = nNode->next_sibling())
			{
				for (rapidxml::xml_node<>* nnNode = nNode->first_node(); nnNode != NULL;
					nnNode = nnNode->next_sibling())
				{
					rapidxml::xml_node<>* Node3 = nnNode->first_node();

					std::string ObjName = nnNode->first_attribute()->value();
					bool isDestructible = false;
					Yield IFDESTROYED;
					sf::Vector2f size;
					sf::IntRect textureCords;
					sf::FloatRect collisionBox;

					for (Node3; Node3 != NULL; Node3 = Node3->next_sibling())
					{
						std::string parametrName(Node3->name());
						rapidxml::xml_node<>* Node4 = Node3->first_node();
						if (parametrName == "ISDESTRUCTIBLE")
						{
							if (Node4->value() == "1")
							{
								isDestructible = true;
							}
						}

						for (Node4; Node4 != NULL; Node4 = Node4->next_sibling())
						{
							std::string nodeName = Node4->name();
							if (nodeName == "DROP")
							{
								IFDESTROYED.first = std::string(Node4->value());
							}
							else if (nodeName == "AMOUNT")
							{
								IFDESTROYED.second
									= static_cast<unsigned>(std::stoi(std::string(Node4->value())));
							}
							else if (nodeName == "X")
							{
								size.x = static_cast<float>(std::stoi(std::string(Node4->value())));
							}
							else if (nodeName == "Y")
							{
								size.y = static_cast<float>(std::stoi(std::string(Node4->value())));
							}
							else if (nodeName == "TOP")
							{
								if (parametrName == "COLLISION")
									collisionBox.top
									= static_cast<float>(std::stoi(std::string(Node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.top = std::stoi(std::string(Node4->value()));
							}
							else if (nodeName == "LEFT")
							{
								if (parametrName == "COLLISION")
									collisionBox.left
									= static_cast<float>(std::stoi(std::string(Node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.left = std::stoi(std::string(Node4->value()));
							}
							else if (nodeName == "WIDTH")
							{
								if (parametrName == "COLLISION")
									collisionBox.width
									= static_cast<float>(std::stoi(std::string(Node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.width = std::stoi(std::string(Node4->value()));
							}
							else if (nodeName == "HEIGHT")
							{
								if (parametrName == "COLLISION")
									collisionBox.height
									= static_cast<float>(std::stoi(std::string(Node4->value())));
								else if (parametrName == "GRAPHICS")
									textureCords.height = std::stoi(std::string(Node4->value()));
							}
						}
					}
					Objects.push_back(ObjectDef(ObjName,size,textureCords,
						collisionBox,IFDESTROYED,isDestructible));
				}
			}
		}
		Objects.shrink_to_fit();

		return Objects;
	}
	//void LoadItemDefFromFile(){}
};