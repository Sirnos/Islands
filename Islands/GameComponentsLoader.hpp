#pragma once


#include <vector>
#include <rapidxml/rapidxml.hpp>
#include <boost/filesystem.hpp>

#include "TypesFromText.hpp"
#include "GameComponents.hpp"


#if defined _WIN32

	const boost::filesystem::path SLASH = "\\";

	const boost::filesystem::path MODS_DIR = "\\Data\\Mods\\";
	const boost::filesystem::path MOD_GRAPHICS_OBJECTS_FILE = "Graphics\\Objects\\Objects.png";
	const boost::filesystem::path MOD_GRAPHICS_ITEMS_FILE = "Graphics\\Items\\Items.png";
	const boost::filesystem::path MOD_GRAPHICS_ENTITIES_FILE = "Graphics\\Entities\\Entities.png";
	const boost::filesystem::path MOD_WORLD_DEFS_DIR = "World\\";
	const boost::filesystem::path MOD_RECIPES_DEFS_DIR = "Recipes\\";
	const boost::filesystem::path MOD_STRUCTURES_DEFS_DIR = "World\\Structures\\";
	const boost::filesystem::path SETTINGS_DIR = "\\Data\\Settings\\";
#else

	const boost::filesystem::path SLASH = "/";

	const boost::filesystem::path MODS_DIR = "/Data/Mods/";
	const boost::filesystem::path MOD_GRAPHICS_OBJECTS_FILE = "Graphics/Objects/Objects.png";
	const boost::filesystem::path MOD_GRAPHICS_ITEMS_FILE = "Graphics/Items/Items.png";
	const boost::filesystem::path MOD_GRAPHICS_ENTITIES_FILE = "Graphics/Entities/Entities.png";
	const boost::filesystem::path MOD_WORLD_DEFS_DIR = "/World/";
	const boost::filesystem::path MOD_RECIPES_DEFS_DIR = "Recipes/";
	const boost::filesystem::path MOD_STRUCTURES_DEFS_DIR = "World/Structures/";
	const boost::filesystem::path SETTINGS_DIR = "Data/Settings/";
#endif

inline StructureInfoKey getStructureInfoKeyFromString(const std::string &str)
{
	return StructureInfoKey(std::stoul(str.substr(0, str.find(','))), str.substr(str.find(',') + 1, str.size() - 1));
}

struct TexturePack
{
	std::string fromFile;
	std::vector<sf::IntRect> TexturesCoords;
};

class GameComponentsLoader
{
	boost::filesystem::path modpath;

	GameComponents *ptr;
	std::vector<BiomeValuesDef> Biomes;
	std::vector<std::pair<std::string, std::vector<RecipeDef>>> Recipes;
	std::vector<StructureDef> Structures;


	TexturePack ObjectsTextures;
	TexturePack ItemsTextures;


	void loadStructureDefFromFile(const boost::filesystem::path &file)
	{
		rapidxml::file<> StructureFile(file.string().data());
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
		Structures.push_back(StructureDef(Name, SpawnChance, Data, Info));
	}
	void loadRecipeDefFromFile(const boost::filesystem::path &file)
	{
		rapidxml::file<char> rfile(file.string().data());
		rapidxml::xml_document<> document;
		document.parse<0>(rfile.data());

		for (rapidxml::xml_node<> *mainNode = document.first_node(); mainNode != nullptr;
			mainNode = mainNode->next_sibling())
		{
			Recipes.push_back(std::pair<std::string, std::vector<RecipeDef>>(file.stem().string(), std::vector<RecipeDef>()));

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
					else if (paramName == "In")
					{
						for (rapidxml::xml_node<>* recipeIn = recipeParamNode->first_node(); recipeIn != nullptr;
							recipeIn = recipeIn->next_sibling())
						{
							newRecipeIn.push_back(getStringAndTpairFromString<unsigned>(std::string(recipeIn->value())));
						}
					}

				}
				Recipes.back().second.push_back(RecipeDef(newRecipeOut, newRecipeIn));
				newRecipeIn.clear();
				newRecipeOut = std::pair<std::string, unsigned>();
			}
		}
	}


	void loadEntities()
	{
		using namespace boost::filesystem;
		path pathToEntities = modpath;
		pathToEntities += "Entities.xml";

		if (exists(pathToEntities))
		{
			rapidxml::file<> File(pathToEntities.string().data());
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
						else if (AttribName == "Size")
						{
							EntitySize = getVectorFromString<float>(std::string(EntityAttribs->value()));
						}
						else if (AttribName == "Stats")
						{
							rapidxml::xml_node<>* EntityStats = EntityAttribs->first_node();
							for (EntityStats; EntityStats != nullptr; EntityStats = EntityStats->next_sibling())
							{
								std::string StatName = EntityStats->name();
								if (StatName == "Hp")
								{
									EntityHp = std::stof(std::string(EntityStats->value()));
								}
								else if (StatName == "Mp")
								{
									EntityMp = std::stof(std::string(EntityStats->value()));
								}
								else if (StatName == "Speed")
								{
									EntitySpeed = std::stof(std::string(EntityStats->value()));
								}
							}
						}
						else if (AttribName == "Behavior")
						{
							rapidxml::xml_node<>* EntityBehaviorNode = EntityAttribs->first_node();
							for (EntityBehaviorNode; EntityBehaviorNode != nullptr; EntityBehaviorNode = EntityBehaviorNode->next_sibling())
							{
								std::string BehaviorVarName = EntityBehaviorNode->name();
								if (BehaviorVarName == "Aggressive")
								{
									EntityBehavior.aggresion = static_cast<EntityAggressive>(std::stoul(std::string(EntityBehaviorNode->value())));
								}
								else if (BehaviorVarName == "CanGroup")
								{
									EntityBehavior.canLiveInGroup = getBoolFromString(std::string(EntityBehaviorNode->value()));
								}
								else if (BehaviorVarName == "GroupSize")
								{
									EntityBehavior.maxGroupSize = std::stoul(std::string(EntityBehaviorNode->value()));
								}
								else if (BehaviorVarName == "AttackType")
								{
									if (std::string(EntityBehaviorNode->value()) == "Distance")
									{
										EntityBehavior.attackPrefer = EntityAttackType::Distance;
									}
								}
							}
						}
					}
					ptr->getEntities()->pushNewDef(MonsterEntityDef(EntityName, EntityStats(EntityHp, EntityMp, EntitySpeed), EntityBehavior, EntitySize));
				}
			}
		}
	}
	void loadStructures()
	{
		using namespace boost::filesystem;
		path pathToStructures = modpath;
		pathToStructures += MOD_STRUCTURES_DEFS_DIR;

		if (is_directory(pathToStructures))
		{
			for (directory_iterator it(pathToStructures); it != directory_iterator(); ++it)
			{
				if (is_regular_file(it->path()))
				{
					if (extension(it->path()) == ".xml")
					{
						loadStructureDefFromFile(it->path());
					}
				}
			}
		}
	}
	void loadBiomes()
	{
		using namespace boost::filesystem;
		path pathToBiomesFile = modpath;
		pathToBiomesFile += MOD_WORLD_DEFS_DIR;
		pathToBiomesFile += "MapGenerator.xml";

		if (exists(pathToBiomesFile))
		{
			rapidxml::file<> File(pathToBiomesFile.string().data());
			rapidxml::xml_document<> Doc;
			Doc.parse<0>(File.data());

			rapidxml::xml_node<> * Base = Doc.first_node();
			for (Base; Base != nullptr; Base = Base->next_sibling())
			{
				rapidxml::xml_node<> * Biome = Base->first_node();
				for (Biome; Biome != nullptr; Biome = Biome->next_sibling())
				{
					Biomes.push_back(BiomeValuesDef());
					Biomes.back().BiomeName = StringToTerrainType(std::string(Biome->first_attribute()->value()));
					rapidxml::xml_node<> * BiomeVars = Biome->first_node();
					for (BiomeVars; BiomeVars != nullptr; BiomeVars = BiomeVars->next_sibling())
					{
						std::string BiomeVarsName = std::string(BiomeVars->name());

						if (BiomeVarsName == "Terrain")
						{
							rapidxml::xml_node<> * Terrain = BiomeVars->first_node();
							for (Terrain; Terrain != nullptr; Terrain = Terrain->next_sibling())
							{
								Biomes.back().TerrainTiles.push_back(std::pair<TerrainType, float>(TerrainType::Null, 0.0f));
								Biomes.back().TerrainTiles.back().first = StringToTerrainType(std::string(Terrain->first_attribute()->value()));
								Biomes.back().TerrainTiles.back().second = std::stof(std::string(Terrain->first_node()->value()));
							}
						}
						else if (BiomeVarsName == "Structures")
						{
							rapidxml::xml_node<> *Structures = BiomeVars->first_node();
							for (Structures; Structures != nullptr; Structures = Structures->next_sibling())
							{
								Biomes.back().SpawnableStructures.push_back(std::pair<std::string, unsigned>());
								Yield strct = getStringAndTpairFromString<unsigned>(std::string(Structures->value()));
								if (strct.first != "" && strct.second != 0)
								{
									Biomes.back().SpawnableStructures.back() = strct;
								}

							}
						}
						else if (BiomeVarsName == "Flora")
						{
							rapidxml::xml_node<> * FloraVars = BiomeVars->first_node();
							for (FloraVars; FloraVars != nullptr; FloraVars = FloraVars->next_sibling())
							{
								Biomes.back().SpawnableObjects.push_back(std::tuple<std::string, float, TerrainType>());
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
									else if (FloraObjVarName == "SpawnOn")
									{
										SpawnTerrain = StringToTerrainType(std::string(FloraObjVars->value()));
									}
								}
								Biomes.back().SpawnableObjects.back() = std::tuple<std::string, float, TerrainType>(SpawnableObjName, SpawnChance, SpawnTerrain);
							}

						}
						else if (BiomeVarsName == "Creatures")
						{

						}
					}
				}
			}
			Biomes.shrink_to_fit();
		}
	}
	void loadObjects()
	{
		ObjectDefContainer *Objects = ptr->getObjects().get();

		using namespace boost::filesystem;
		path pathToObject = modpath;
		pathToObject += "Objects.xml";

		if (exists(pathToObject))
		{
			rapidxml::file<char> File(pathToObject.string().data());
			rapidxml::xml_document<> document;
			document.parse<0>(File.data());

			path pathToObjText(modpath);
			pathToObjText += MOD_GRAPHICS_OBJECTS_FILE;
			if (exists(pathToObjText))
			{
				ObjectsTextures.fromFile = pathToObjText.string();
			}

			for (rapidxml::xml_node<>* mainNode = document.first_node(); mainNode != nullptr;
				mainNode = mainNode->next_sibling())
			{
				std::vector<unsigned> tempUint;
				std::vector<float> tempFloat;
				std::vector<std::string> tempString;

				rapidxml::xml_node<>* objectsNode = mainNode->first_node();
				for (objectsNode; objectsNode != nullptr; objectsNode = objectsNode->next_sibling())
				{
					std::string newObjName = objectsNode->first_attribute()->value();
					bool newObjDestructible = false;
					bool newObjCollision = false;
					ObjYield newObjYield;
					sf::IntRect newObjTextureCoord;
					ObjectType newObjType = ObjectType::Default;

					rapidxml::xml_node<>* objectsParamNode = objectsNode->first_node();
					for (objectsParamNode; objectsParamNode != nullptr; objectsParamNode = objectsParamNode->next_sibling())
					{
						std::string paramName = objectsParamNode->name();

						if (paramName == "Graphics")
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
							rapidxml::xml_node<char> *listNode = objectsParamNode->first_node();
							for (listNode; listNode != nullptr; listNode = listNode->next_sibling())
							{
								newObjYield.push_back(getStringAndTpairFromString<unsigned>(std::string(listNode->value())));
							}
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
							tempString.push_back(std::string(objectsParamNode->value()));
						}
						else if (paramName == "TreeDef")
						{
							newObjType = ObjectType::Tree;
						}
					}

					bool success = false;
					switch (newObjType)
					{
					case ObjectType::Default:
						Objects->pushNewDef(new ObjectDef(newObjName, newObjYield, newObjDestructible, newObjCollision));
						success = true;
						break;
					case ObjectType::Chest:
						Objects->pushNewDef(new ChestDef(newObjName, newObjCollision, newObjYield, newObjDestructible, tempUint.back()));
						success = true;
						break;
					case ObjectType::Tree:
						Objects->pushNewDef(new ObjectDef(newObjName, newObjYield, newObjDestructible, newObjCollision, ObjectType::Tree));
						success = true;
						break;
					case ObjectType::Sapling:
						Objects->pushNewDef(new SaplingDef(newObjName, newObjCollision, newObjYield, newObjDestructible, tempFloat.back(), tempString.back()));
						success = true;
						break;
					case ObjectType::Spawner:
						break;
					case ObjectType::CraftingPlace:
						for (const auto & rec : Recipes)
						{
							if (rec.first == tempString.back())
							{
								Objects->pushNewDef(new CraftingPlaceDef(newObjName, newObjYield, newObjDestructible, newObjCollision, rec.second));
								success = true;
								break;
							}
							else
							{
								ErrorHandler::logToFile("Cannot find recipe for " + newObjName + " object");
							}
						}
						break;
					default:
						break;
					}
					if (success)
					{
						ptr->getItems()->pushNewDef(new PlaceableDef(newObjName));
					}

					ObjectsTextures.TexturesCoords.push_back(newObjTextureCoord);
					tempFloat.clear();
					tempString.clear();
					tempUint.clear();
					newObjYield.clear();
				}
			}
		}
	}
	void loadItems()
	{
		using namespace boost::filesystem;
		path pathToItemsDefs = modpath;
		pathToItemsDefs += "Items.xml";

		if (exists(pathToItemsDefs))
		{
			ItemDefContainer * Items = ptr->getItems().get();

			rapidxml::file<char> File(pathToItemsDefs.string().data());
			rapidxml::xml_document<> document;
			document.parse<0>(File.data());

			for (rapidxml::xml_node<>* mainNode = document.first_node(); mainNode != nullptr;
				mainNode = mainNode->next_sibling())
			{
				unsigned textureSize;

				rapidxml::xml_attribute<>* mainAttrib = mainNode->first_attribute();
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
						Items->pushNewDef(new RawMaterialDef(newItemName, newItemMaxStack));
						break;
					case ItemType::Placeable:
						break;
					case ItemType::MeleeWeapon:
						Items->pushNewDef(new MeleeWeaponDef(newItemName, IntTemp.back(), FloatTemp.back()));
						break;
					case ItemType::DistanceWeapon:
						Items->pushNewDef(new DistantWeaponDef(newItemName, IntTemp.back(), FloatTemp.back()));
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

						Items->pushNewDef(new ArmorDef(newItemName, newArmorPart, IntTemp.back()));
						break;
					default:
						break;
					}

					IntTemp.clear();
					FloatTemp.clear();
					StringTemp.clear();
					ItemsTextures.TexturesCoords.push_back(sf::IntRect(texturePos.x * textureSize, texturePos.y * textureSize, textureSize, textureSize));
				}
			}
		}
	}
	void loadRecipesDef()
	{
		using namespace boost::filesystem;
		path pathToRecipes = modpath;
		pathToRecipes += MOD_RECIPES_DEFS_DIR;

		if (is_directory(pathToRecipes))
		{
			for (directory_iterator it(pathToRecipes); it != directory_iterator(); it++)
			{
				if (is_regular_file(it->path()))
				{
					if (extension(it->path()) == ".xml")
					{
						loadRecipeDefFromFile(it->path());
					}
				}
			}
		}
	}

public:
	GameComponentsLoader() = delete;
	GameComponentsLoader(const std::string &folderName, GameComponents &components)
		:modpath(boost::filesystem::current_path()), ptr(&components)
	{
		modpath += MODS_DIR;
		modpath += folderName;
		modpath += SLASH;
		ObjectsTextures.fromFile = modpath.string() + MOD_GRAPHICS_OBJECTS_FILE.string();
		ItemsTextures.fromFile = modpath.string() + MOD_GRAPHICS_ITEMS_FILE.string();
	}
	~GameComponentsLoader() = default;

	bool exist() const
	{
		return boost::filesystem::exists(modpath);
	}
	void load()
	{
		if (!exist())
		{
			ErrorHandler::logToFile("Couldn't load mod from " + modpath.string());
		}
		loadRecipesDef();
		loadObjects();
		loadItems();
		loadStructures();
		loadBiomes();
		loadEntities();
	}
	const boost::filesystem::path & getModPath() const
	{
		return modpath;
	}

	const TexturePack &getItemsTextures() const
	{
		return ItemsTextures;
	}
	const TexturePack &getObjectsTextures() const
	{
		return ObjectsTextures;
	}


	const std::vector<BiomeValuesDef> &getBiomes() const
	{
		return Biomes;
	}
	const std::vector<std::pair<std::string, std::vector<RecipeDef>>> &getRecipes() const
	{
		return Recipes;
	}
	const std::vector<StructureDef> &getStructures() const
	{
		return Structures;
	}


	static void loadTerrainTextureCoords(std::vector<sf::IntRect> & TerrainTextCoords)
	{
		using namespace boost::filesystem;

		path pathToTerrainTextCoords = current_path(); 
		pathToTerrainTextCoords += SETTINGS_DIR.string();
        pathToTerrainTextCoords += "Terrain.xml";

		rapidxml::file<> File(pathToTerrainTextCoords.string().data());
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
};
