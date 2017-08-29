#pragma once

#include <vector>

#include <rapidxml_utils.hpp>
#include <rapidxml.hpp>

#include "SFMLTypesFromText.hpp"
#include "DefContainer.hpp"

class GameComponentsLoader
{
	static Yield getYieldFromString(const std::string &str)
	{
		return Yield(str.substr(0, str.find(',')), std::stoul(str.substr(str.find(',') + 1, str.size() - 1)));
	}
	static StructureInfoKey getStructureInfoKeyFromString(const std::string &str)
	{
		return StructureInfoKey(std::stoul(str.substr(0, str.find(','))), str.substr(str.find(',') + 1, str.size() - 1));
	}

	static StructureDef LoadStructureDefFromFile(const std::string &file)
	{
		rapidxml::file<> StructureFile(file.data());
		rapidxml::xml_document<> StructureDoc;
		StructureDoc.parse<0>(StructureFile.data());

		std::string Name;
		unsigned SpawnChance;
		StructureInfo Info;
		StructureData Data;

		rapidxml::xml_node<> *BaseNode = StructureDoc.first_node();
		for (BaseNode; BaseNode != nullptr; BaseNode = BaseNode->next_sibling())
		{
			rapidxml::xml_node<> *ParamsNode = BaseNode->first_node();
			for (ParamsNode; ParamsNode != nullptr; ParamsNode = ParamsNode->next_sibling())
			{
				std::string ParamName = ParamsNode->name();
				if (ParamName == "Name")
				{
					Name = std::string(ParamsNode->value());
				}
				else if (ParamName == "SpawnChance")
				{
					SpawnChance = std::stoul(std::string(ParamsNode->value()));
				}
				else if (ParamName == "Info")
				{
					rapidxml::xml_node<> *InfoParamNode = ParamsNode->first_node();
					for (InfoParamNode; InfoParamNode != nullptr; InfoParamNode = InfoParamNode->next_sibling())
					{
						Info.push_back(getStructureInfoKeyFromString(std::string(InfoParamNode->value())));
					}
				}
				else if (ParamName == "Data")
				{
					rapidxml::xml_node<> *DataNode = ParamsNode->first_node();
					for (DataNode; DataNode != nullptr; DataNode = DataNode->next_sibling())
					{
						Data.push_back(std::vector<unsigned>());
						rapidxml::xml_node<> *DataValues = DataNode->first_node();
						for (DataValues; DataValues != nullptr; DataValues = DataValues->next_sibling())
						{
							Data.back().push_back(std::stoul(std::string(DataValues->value())));
						}
					}
				}
			}
		}
		return StructureDef{ Name, SpawnChance, Data, Info };
	}

public:
	static void LoadObjectDefFromFile(std::vector<ObjectDef*> &Objects,std::string &ObjectsGraphicsFile,std::vector<sf::IntRect> &Textures)
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
			std::vector<RecipeDef> tempRecipes;

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
						newObjSize = getVectorFromString<int>(std::string(objectsParamNode->value()));
					}
					else if(paramName == "Graphics")
					{
						newObjTextureCoord = getRectFromString<int>(std::string(objectsParamNode->value()));
					}
					else if(paramName == "Collision")
					{
						newObjCollisionBox = getRectFromString<float>(std::string(objectsParamNode->value()));
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
					else if(paramName == "CraftingPlaceDef")
					{
						newObjType = ObjectType::CraftingPlace;
						LoadRecipeDefFromFile(tempRecipes,std::string(objectsParamNode->value()));
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
				case ObjectType::CraftingPlace:
					Objects.push_back(new CraftingPlaceDef(newObjName, newObjSize, newObjCollisionBox, newObjYield, newObjDestructible, tempRecipes));
					break;
				default:
					break;
				}
				Textures.push_back(newObjTextureCoord);
				tempFloat.clear();
				tempString.clear();
				tempUint.clear();
				tempRecipes.clear();
			}
		}
		Objects.shrink_to_fit();
	}

	static void GenerateItemsFromObjectDef(std::vector<ObjectDef*> &Objs,std::vector<ItemDef*> &Items)
	{
		for (unsigned i = 0; i < Objs.size(); i++)
		{
			Items.push_back(new PlaceableDef(Objs[i]->getName()));
		}
		Items.shrink_to_fit();
	}

	static void LoadItemDefFromFile(std::vector<ItemDef*> &Items,std::string &ItemsGraphicsFile,std::vector<sf::IntRect> &textures)
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
				std::vector<int> IntTemp;
				std::vector<float> FloatTemp;
				std::vector<std::string> StringTemp;
				ArmorPart newArmorPart = ArmorPart::Head;

				ItemType newItemType = ItemType::Default;

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
						texturePos = getVectorFromString<int>(std::string(itemParamNode->value()));
					}
					else if(paramName == "MaxStack")
					{
						newItemMaxStack = std::stoul(std::string(itemParamNode->value()));
					}
					else if(paramName == "MeleeWeaponDef")
					{
						newItemType = ItemType::MeleeWeapon;
						IntTemp.push_back(std::stoi(std::string(itemParamNode->first_node()->value())));
						FloatTemp.push_back(std::stof(std::string(itemParamNode->first_node()->next_sibling()->value())));
					}
					else if(paramName == "DistantWeaponDef")
					{
						newItemType = ItemType::DistanceWeapon;
						IntTemp.push_back(std::stoi(std::string(itemParamNode->first_node()->value())));
						FloatTemp.push_back(std::stof(std::string(itemParamNode->first_node()->next_sibling()->value())));
					}
					else if(paramName == "ArmorDef")
					{
						newItemType = ItemType::Armor;
						IntTemp.push_back(std::stoi(std::string(itemParamNode->first_node()->value())));
						StringTemp.push_back(std::string(itemParamNode->first_node()->next_sibling()->value()));
					}
				}

				switch (newItemType)
				{
				case ItemType::Default:
					Items.push_back(new RawMaterialDef(newItemName, newItemMaxStack));
					break;
				case ItemType::Placeable:
					break;
				case ItemType::MeleeWeapon:
					Items.push_back(new MeleeWeaponDef(newItemName, IntTemp.back(), FloatTemp.back()));
					break;
				case ItemType::DistanceWeapon:
					Items.push_back(new DistantWeaponDef(newItemName, IntTemp.back(), FloatTemp.back()));
					break;
				case ItemType::Armor:
					if (StringTemp.back() == "Head"){}
					else if(StringTemp.back() == "Chest")
					{
						newArmorPart = ArmorPart::Chest;
					}
					else if(StringTemp.back() == "Legs")
					{
						newArmorPart = ArmorPart::Legs;
					}

					Items.push_back(new ArmorDef(newItemName, newArmorPart, IntTemp.back()));
					break;
				default:
					break;
				}

				IntTemp.clear();
				FloatTemp.clear();
				StringTemp.clear();
				textures.push_back(sf::IntRect(texturePos.x * textureSize, texturePos.y * textureSize, textureSize, textureSize));
			}
			Items.shrink_to_fit();
		}
	}

	static void LoadRecipeDefFromFile(std::vector<RecipeDef> &recipes,const std::string &file)
	{
		rapidxml::file<char> rfile(file.data());
		rapidxml::xml_document<> document;
		document.parse<0>(rfile.data());

		for (rapidxml::xml_node<> *mainNode = document.first_node(); mainNode != nullptr;
			mainNode = mainNode->next_sibling())
		{
			RecipeElement newRecipeOut;
			std::vector<RecipeElement> newRecipeIn;
			for (rapidxml::xml_node<> *recipeNode = mainNode->first_node(); recipeNode != nullptr;
				recipeNode = recipeNode->next_sibling())
			{

				for (rapidxml::xml_node<> *recipeParamNode = recipeNode->first_node(); recipeParamNode != nullptr;
					recipeParamNode = recipeParamNode->next_sibling())
				{
					std::string paramName = recipeParamNode->name();

					if (paramName == "Out")
					{
						newRecipeOut = getYieldFromString(std::string(recipeParamNode->value()));
					}
					else if(paramName == "In")
					{
						for (rapidxml::xml_node<>* recipeIn = recipeParamNode->first_node(); recipeIn != nullptr;
							recipeIn = recipeIn->next_sibling())
						{
							newRecipeIn.push_back(getYieldFromString(std::string(recipeIn->value())));
						}
					}

				}
				recipes.push_back(RecipeDef(newRecipeOut,newRecipeIn));
				newRecipeIn.clear();
			}
		}
		recipes.shrink_to_fit();
	}

	static void LoadStructuresDef(std::vector<StructureDef> & Structures)
	{
		rapidxml::file<> File("Data/World/Structures.xml");
		rapidxml::xml_document<> Doc;
		Doc.parse<0>(File.data());

		rapidxml::xml_node<> *BaseNode = Doc.first_node();
		for (BaseNode; BaseNode != nullptr; BaseNode = BaseNode->next_sibling())
		{
			rapidxml::xml_node<> *StructuresNode = BaseNode->first_node();
			for (StructuresNode; StructuresNode != nullptr; StructuresNode = StructuresNode->next_sibling())
			{
				std::string StructureFile = "Data/World/Structures/";
				StructureFile += std::string(StructuresNode->value());
				Structures.push_back(LoadStructureDefFromFile(StructureFile));
			}
		}
	}

	static void LoadTerrainTextureCoords(std::vector<sf::IntRect> & TerrainTextCoords)
	{
		rapidxml::file<> File("Data/Terrain.xml");
		rapidxml::xml_document<> Doc;
		Doc.parse<0>(File.data());

		rapidxml::xml_node<>* BaseNode = Doc.first_node();
		for (BaseNode; BaseNode != nullptr; BaseNode = BaseNode->next_sibling())
		{
			rapidxml::xml_node<>* TerrainNode = BaseNode->first_node();
			for (TerrainNode; TerrainNode != nullptr; TerrainNode = TerrainNode->next_sibling())
			{
				TerrainTextCoords.push_back(getRectFromString<int>(std::string(TerrainNode->value())));
			}
		}
	}

	static void LoadLocalMapVariables(std::vector<LocalMapVariablesDef> & Vars)
	{
		rapidxml::file<> File("Data/World/MapGenerator.xml");
		rapidxml::xml_document<> Doc;
		Doc.parse<0>(File.data());

		rapidxml::xml_node<> * Base = Doc.first_node();
		for (Base; Base != nullptr; Base = Base->next_sibling())
		{
			rapidxml::xml_node<> * Biome = Base->first_node();
			for (Biome; Biome != nullptr; Biome = Biome->next_sibling())
			{
				Vars.push_back(LocalMapVariablesDef());
				Vars.back().Biome = StringToTerrainType(std::string(Biome->first_attribute()->value()));
				rapidxml::xml_node<> * BiomeVars = Biome->first_node();
				for (BiomeVars; BiomeVars != nullptr; BiomeVars = BiomeVars->next_sibling())
				{
					std::string BiomeVarsName = std::string(BiomeVars->name());

					if (BiomeVarsName == "Terrain")
					{
						rapidxml::xml_node<> * Terrain = BiomeVars->first_node();
						for (Terrain; Terrain != nullptr; Terrain = Terrain->next_sibling())
						{
							Vars.back().TerrainTiles.push_back(std::pair<TerrainType, float>(TerrainType::Null, 0.0f));
							Vars.back().TerrainTiles.back().first = StringToTerrainType(std::string(Terrain->first_attribute()->value()));
							Vars.back().TerrainTiles.back().second = std::stof(std::string(Terrain->first_node()->value()));
						}
					}
					else if(BiomeVarsName == "Structures")
					{

					}
					else if(BiomeVarsName == "Flora")
					{
						rapidxml::xml_node<> * FloraVars = BiomeVars->first_node();
						for (FloraVars; FloraVars != nullptr; FloraVars = FloraVars->next_sibling())
						{
							Vars.back().SpawnableObjects.push_back(std::tuple<std::string, float,TerrainType>());
							rapidxml::xml_node<> *FloraObjVars = FloraVars->first_node();

							std::string SpawnableObjName = FloraVars->first_attribute()->value();
							float SpawnChance;
							TerrainType SpawnTerrain;

							for (FloraObjVars; FloraObjVars != nullptr; FloraObjVars = FloraObjVars->next_sibling())
							{
								std::string FloraObjVarName(FloraObjVars->name());
								if (FloraObjVarName == "Chance")
								{
									SpawnChance = std::stof(std::string(FloraObjVars->value()));
								}
								else if(FloraObjVarName == "SpawnOn")
								{
									SpawnTerrain = StringToTerrainType(std::string(FloraObjVars->value()));
								}
							}
							Vars.back().SpawnableObjects.back() = std::tuple<std::string, float, TerrainType>(SpawnableObjName, SpawnChance, SpawnTerrain);
						}

					}
					else if(BiomeVarsName == "Creatures")
					{

					}
				}
			}
		}
	}
};