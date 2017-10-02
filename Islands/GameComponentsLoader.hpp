#pragma once

#include <vector>

#include <rapidxml/rapidxml.hpp>

#include "TypesFromText.hpp"
#include "DefContainer.hpp"

class GameComponentsLoader
{
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
	static void LoadObjectDefFromFile(std::vector<ObjectDef*> &Objects, std::string &ObjectsGraphicsFile, std::vector<sf::IntRect> &Textures)
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

			rapidxml::xml_node<>* objectsNode = mainNode->first_node();
			for (objectsNode; objectsNode != nullptr; objectsNode = objectsNode->next_sibling())
			{
				std::string newObjName = objectsNode->first_attribute()->value();
				bool newObjDestructible = false;
				bool newObjCollision = false;
				Yield newObjYield;
				sf::Vector2i newObjSize;
				sf::IntRect newObjTextureCoord;
				ObjectType newObjType = ObjectType::Default;

				rapidxml::xml_node<>* objectsParamNode = objectsNode->first_node();
				for (objectsParamNode; objectsParamNode != nullptr; objectsParamNode = objectsParamNode->next_sibling())
				{
					std::string paramName = objectsParamNode->name();

					if (paramName == "Size")
					{
						newObjSize = getVectorFromString<int>(std::string(objectsParamNode->value()));
					}
					else if (paramName == "Graphics")
					{
						newObjTextureCoord = getRectFromString<int>(std::string(objectsParamNode->value()));
					}
					else if (paramName == "Collision")
					{
						newObjCollision = getBoolFromString(std::string(objectsParamNode->value()));
					}
					else if (paramName == "Isdestructible")
					{
						newObjDestructible = getBoolFromString(objectsParamNode->value());
					}
					else if (paramName == "Ifdestroyed")
					{
						newObjYield = getStringAndTpairFromString<unsigned>(std::string(objectsParamNode->value()));
					}
					else if (paramName == "ChestDef")
					{
						newObjType = ObjectType::Chest;
						tempUint.push_back(std::stoul(std::string(objectsParamNode->first_node()->value())));
					}
					else if (paramName == "SaplingDef")
					{
						newObjType = ObjectType::Sapling;
						tempFloat.push_back(std::stof(std::string(objectsParamNode->first_node()->value())));
						tempString.push_back(std::string(objectsParamNode->first_node()->next_sibling()->value()));
					}
					else if (paramName == "CraftingPlaceDef")
					{
						newObjType = ObjectType::CraftingPlace;
						LoadRecipeDefFromFile(tempRecipes, std::string(objectsParamNode->value()));
					}
				}

				switch (newObjType)
				{
				case ObjectType::Default:
					Objects.push_back(new ObjectDef(newObjName, newObjSize, newObjYield, newObjDestructible, newObjCollision));
					break;
				case ObjectType::Chest:
					Objects.push_back(new ChestDef(newObjName, newObjSize, newObjCollision, newObjYield, newObjDestructible, tempUint.back()));
					break;
				case ObjectType::Tree:
					break;
				case ObjectType::Sapling:
					Objects.push_back(new SaplingDef(newObjName, newObjSize, newObjCollision, newObjYield, newObjDestructible, tempFloat.back(), tempString.back()));
					break;
				case ObjectType::Spawner:
					break;
				case ObjectType::CraftingPlace:
					Objects.push_back(new CraftingPlaceDef(newObjName, newObjSize, newObjYield, newObjDestructible, newObjCollision, tempRecipes));
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

	static void GenerateItemsFromObjectDef(const std::vector<ObjectDef*> &Objs, std::vector<ItemDef*> &Items)
	{
		for (unsigned i = 0; i < Objs.size(); i++)
		{
			Items.push_back(new PlaceableDef(Objs[i]->getName()));
		}
		Items.shrink_to_fit();
	}

	static void LoadItemDefFromFile(std::vector<ItemDef*> &Items, std::string &ItemsGraphicsFile, std::vector<sf::IntRect> &textures)
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
					else if (paramName == "MaxStack")
					{
						newItemMaxStack = std::stoul(std::string(itemParamNode->value()));
					}
					else if (paramName == "MeleeWeaponDef")
					{
						newItemType = ItemType::MeleeWeapon;
						IntTemp.push_back(std::stoi(std::string(itemParamNode->first_node()->value())));
						FloatTemp.push_back(std::stof(std::string(itemParamNode->first_node()->next_sibling()->value())));
					}
					else if (paramName == "DistantWeaponDef")
					{
						newItemType = ItemType::DistanceWeapon;
						IntTemp.push_back(std::stoi(std::string(itemParamNode->first_node()->value())));
						FloatTemp.push_back(std::stof(std::string(itemParamNode->first_node()->next_sibling()->value())));
					}
					else if (paramName == "ArmorDef")
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
					if (StringTemp.back() == "Head") {}
					else if (StringTemp.back() == "Chest")
					{
						newArmorPart = ArmorPart::Chest;
					}
					else if (StringTemp.back() == "Legs")
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
						newRecipeOut = getStringAndTpairFromString<unsigned>(std::string(recipeParamNode->value()));
					}
					else if(paramName == "In")
					{
						for (rapidxml::xml_node<>* recipeIn = recipeParamNode->first_node(); recipeIn != nullptr;
							recipeIn = recipeIn->next_sibling())
						{
							newRecipeIn.push_back(getStringAndTpairFromString<unsigned>(std::string(recipeIn->value())));
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
		Structures.shrink_to_fit();
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
		TerrainTextCoords.shrink_to_fit();
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
						rapidxml::xml_node<> *Structures = BiomeVars->first_node();
						for (Structures; Structures != nullptr; Structures = Structures->next_sibling())
						{
							Vars.back().SpawnableStructures.push_back(std::pair<std::string, unsigned>());
							Yield strct = getStringAndTpairFromString<unsigned>(std::string(Structures->value()));
							if(strct.first != "" && strct.second != 0)
							{
								Vars.back().SpawnableStructures.back() = strct;
							}

						}
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
		Vars.shrink_to_fit();
	}

	static void LoadEntitiesDefFromFile(std::vector<MonsterEntityDef> &Entities)
	{
		rapidxml::file<> File("Data/Entities.xml");
		rapidxml::xml_document<> Doc;
		Doc.parse<0>(File.data());

		rapidxml::xml_node<> *BaseNode = Doc.first_node();
		for (BaseNode; BaseNode != nullptr; BaseNode = BaseNode->next_sibling())
		{
			rapidxml::xml_node<> *EntitiesNode = BaseNode->first_node();
			for (EntitiesNode; EntitiesNode != nullptr; EntitiesNode = EntitiesNode->next_sibling())
			{
				std::string EntityName;
				float EntityHp = 0.0f;
				float EntityMp = 0.0f;
				float EntitySpeed = 0.0f;
				sf::Vector2f EntitySize;
				EntityBehaviorValues EntityBehavior;

				rapidxml::xml_node<> *EntityAttribs = EntitiesNode->first_node();
				for (EntityAttribs; EntityAttribs != nullptr; EntityAttribs = EntityAttribs->next_sibling())
				{
					std::string AttribName = EntityAttribs->name();

					if (AttribName == "Name")
					{
						EntityName = EntityAttribs->value();
					}
					else if(AttribName == "Size")
					{
						EntitySize = getVectorFromString<float>(std::string(EntityAttribs->value()));
					}
					else if(AttribName == "Stats")
					{
						rapidxml::xml_node<>* EntityStats = EntityAttribs->first_node();
						for (EntityStats; EntityStats != nullptr; EntityStats = EntityStats->next_sibling())
						{
							std::string StatName = EntityStats->name();
							if (StatName == "Hp")
							{
								EntityHp = std::stof(std::string(EntityStats->value()));
							}
							else if(StatName == "Mp")
							{
								EntityMp = std::stof(std::string(EntityStats->value()));
							}
							else if(StatName == "Speed")
							{
								EntitySpeed = std::stof(std::string(EntityStats->value()));
							}
						}
					}
					else if(AttribName == "Behavior")
					{
						rapidxml::xml_node<>* EntityBehaviorNode = EntityAttribs->first_node();
						for (EntityBehaviorNode; EntityBehaviorNode != nullptr; EntityBehaviorNode = EntityBehaviorNode->next_sibling())
						{
							std::string BehaviorVarName = EntityBehaviorNode->name();
							if (BehaviorVarName == "Aggressive")
							{
								EntityBehavior.aggresion = static_cast<EntityAggressive>(std::stoul(std::string(EntityBehaviorNode->value())));
							}
							else if(BehaviorVarName == "CanGroup")
							{
								EntityBehavior.canLiveInGroup = getBoolFromString(std::string(EntityBehaviorNode->value()));
							}
							else if(BehaviorVarName == "GroupSize")
							{
								EntityBehavior.maxGroupSize = std::stoul(std::string(EntityBehaviorNode->value()));
							}
							else if(BehaviorVarName == "AttackType")
							{
								if (std::string(EntityBehaviorNode->value()) == "Distance")
								{
									EntityBehavior.attackPrefer = EntityAttackType::Distance;
								}
							}
						}
					}
				}
				Entities.push_back(MonsterEntityDef(EntityName, EntityStats(EntityHp, EntityMp, EntitySpeed), EntityBehavior, EntitySize));
			}
		}
		Entities.shrink_to_fit();
	}
};
