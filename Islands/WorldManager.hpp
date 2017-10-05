#pragma once

#include <SFML/System/Clock.hpp>
#include <memory>
#include "FastNoise/FastNoise.h"

#include "RandomNumberGenerator.hpp"
#include "ErrorHandler.hpp"
#include "DefContainer.hpp"
#include "Object.hpp"
#include "World.hpp"

class WorldManager
{
	std::vector<BiomeValues> Biomes;

	std::shared_ptr<World> ManagementWorld;
	std::shared_ptr<ItemDefContainer> ItemsDef;
	std::shared_ptr<ObjectDefContainer> ObjectsDef;
	std::shared_ptr<sf::Clock> GameClockPtr;
	std::vector<Structure> Structures;

	unsigned StructuresPerLocalMap;
public:
	WorldManager()
		:StructuresPerLocalMap(0)
	{}
	WorldManager(const WorldManager & other) = delete;
	~WorldManager() = default;


	void AssingLocalMapsBuilderVars(std::vector<BiomeValues> &newBiomes)
	{
		Biomes = newBiomes;
	}
	void AssingStructures(std::vector<Structure> &Structs)
	{
		this->Structures = Structs;
	}
	void AssingItemsDef(std::shared_ptr<ItemDefContainer> &Items)
	{
		ItemsDef = Items;
	}
	void AssingObjectsDef(std::shared_ptr<ObjectDefContainer> &ObjectDef)
	{
		this->ObjectsDef = ObjectDef;
	}
	void AssingWorld(std::shared_ptr<World> &World)
	{
		this->ManagementWorld = World;
	}
	void AssingClock(sf::Clock &Clock)
	{
		GameClockPtr = std::make_shared<sf::Clock>(Clock);
	}


	void setStructuresAmountInLocalMap(unsigned Amount)
	{
		if (Amount >= 6) { Amount = 5; }
		StructuresPerLocalMap = Amount;
	}


	void buildLocalMap(TerrainType Base, size_t LocalMapSize = World::DefaultMapSize)
	{
		sf::Clock TestClock;

		FastNoise noiseModule;

		IntegerGenerator<unsigned> Gen;
		noiseModule.SetSeed(Gen.get(1, std::numeric_limits<unsigned>::max() - 1));
		noiseModule.SetNoiseType(FastNoise::Perlin);
		noiseModule.SetFractalOctaves(6);
		noiseModule.SetFrequency(1.0);

		ManagementWorld->resizeLocalMap(LocalMapSize);

		double noise;
		TerrainType Terrain = TerrainType::Null;

		size_t match = 0;
		for (size_t i = 0; i < Biomes.size(); i++)
		{
			if (Base == Biomes[i].BiomeName) { match = i; break; }
		}

		if (match == 0)
		{
			for (size_t x = 0; x < ManagementWorld->getLocalMapSize(); x++)
			{
				for (size_t y = 0; y < ManagementWorld->getLocalMapSize(); y++)
				{
					noise = noiseModule.GetValue(1.25f + (0.1f * x), 0.75f + (0.1f * y), 0.5f);
					if (noise > 0.75)
					{
						Terrain = TerrainType::Rock;
					}
					else if (noise > 0.35)
					{
						Terrain = TerrainType::Dirt;
					}
					else if (noise > -0.25)
					{
						Terrain = TerrainType::Grass;
					}
					else
					{
						Terrain = TerrainType::Water;
					}
					ManagementWorld->setLocalMapTileTerrain(sf::Vector2u(x, y), Terrain);
				}
			}
		}
		else
		{
			float chanceForObjectSpawn = 0.0f;
			FloatGenerator<float> RandomFloat;

			for (size_t x = 0; x < ManagementWorld->getLocalMapSize(); x++)
			{
				for (size_t y = 0; y < ManagementWorld->getLocalMapSize(); y++)
				{
					noise = noiseModule.GetValue(1.25f + (0.1f * x), 0.75f + (0.1f * y), 0.5f);
					ManagementWorld->setLocalMapTileTerrain(sf::Vector2u(x, y), TerrainType::Water);
					for (const auto & terrainChance : Biomes[match].TerrainTiles)
					{
						if (noise > terrainChance.second)
						{
							ManagementWorld->setLocalMapTileTerrain(sf::Vector2u(x, y), terrainChance.first);
							break;
						}
					}

					for (const auto & SpawnObject : Biomes[match].SpawnableObjects)
					{
						size_t ObjectId = std::get<size_t>(SpawnObject);
						float ObjectSpawnChance = std::get<float>(SpawnObject);
						TerrainType ObjectTerrain = std::get<TerrainType>(SpawnObject);

						if (ManagementWorld->getLocalMapTileTerrain(sf::Vector2u(x, y)) == ObjectTerrain)
						{
							chanceForObjectSpawn = RandomFloat.get(0.01f, 1.0f);
							if (chanceForObjectSpawn <= ObjectSpawnChance)
							{
								placeObject(sf::Vector2u(x, y), ObjectId);
							}
						}
					}
				}
			}

			size_t PlacedStructures = 0;
			while (PlacedStructures <= StructuresPerLocalMap)
			{
				sf::Vector2u structurePosition;
				structurePosition.x = Gen.get(0, ManagementWorld->getLocalMapSize());
				structurePosition.y = Gen.get(0, ManagementWorld->getLocalMapSize());

				unsigned dice = Gen.get(0, 100);
				for (auto const & i : Biomes[match].SpawnableStructures)
				{
					if (dice <= i.second )
					{
						placeStructure(structurePosition, i.first);
						PlacedStructures++;
					}
				}
			}

		}

		ErrorHandler::logToFile("Generate Local Map [Size] = " + std::to_string(ManagementWorld->getLocalMapSize()) +
			" [Time] = " + std::to_string(TestClock.getElapsedTime().asMilliseconds()) + " milisecs ");
	}
	void buildWorld(size_t WorldSize = World::DefaultWorldSize, unsigned Seed = 1)
	{}


	bool placeObject(const sf::Vector2u &tile, unsigned ObjectId)
	{
		if (ObjectId >= ObjectsDef->getSize() || ManagementWorld->getLocalMapTileObject(tile) != nullptr)
		{
			return false;
		}
		if (!sf::Rect<unsigned>(sf::Vector2u(),sf::Vector2u(ManagementWorld->getLocalMapSize(),
			ManagementWorld->getLocalMapSize())).contains(tile))
		{
			return false;
		}
		

		switch (ObjectsDef->getDefinition(ObjectId)->getType())
		{
		case ObjectType::Default:
			ManagementWorld->setLocalMapTileObject(tile, new Object(ObjectId, ObjectsDef->getDefinition(ObjectId)->getCollision()));
			return true;
			break;
		case ObjectType::Chest:
			ManagementWorld->setLocalMapTileObject(tile, new ChestObject(ObjectId,
				dynamic_cast<ChestDef*>(ObjectsDef->getDefinition(ObjectId))->getCapacity()));
			return true;
			break;
		case ObjectType::CraftingPlace:
			ManagementWorld->setLocalMapTileObject(tile, new CraftingPlaceObject(ObjectId, 
				makeFromDef::makeRecipe(dynamic_cast<CraftingPlaceDef*>(ObjectsDef->getDefinition(ObjectId))->getRecipes(),*ItemsDef.get())));
			return true;
			break;
		case ObjectType::Tree:
			break;
		case ObjectType::Sapling:
			ManagementWorld->setLocalMapTileObject(tile, new SaplingObject(ObjectId, GameClockPtr->getElapsedTime().asSeconds()));
			return true;
			break;
		case ObjectType::Spawner:
			break;
		default:
			break;
		}
		return false;
	}
	bool placeStructure(const sf::Vector2u &begin, unsigned StructureId)
	{
		if (StructureId >= Structures.size())
		{
			return false;
		}
		if (!sf::Rect<unsigned>(sf::Vector2u(), sf::Vector2u(ManagementWorld->getLocalMapSize(),
			ManagementWorld->getLocalMapSize())).contains(begin))
		{
			return false;
		}

		sf::Vector2u tile = begin;

		for (auto & x : Structures[StructureId].getData())
		{
			if (tile.x >= ManagementWorld->getLocalMapSize()) { break; }
			for (auto & y : x)
			{
				if (tile.y >= ManagementWorld->getLocalMapSize()) { break; }

				ManagementWorld->removeLocalMapTileObject(tile);
				placeObject(tile, y);
				tile.y++;
			}
			tile.x++;
			tile.y = begin.y;
		}

		return true;
	}


	std::vector<ItemField> getItemsFromChestObject(const sf::Vector2u &tile) const
	{
		std::vector<ItemField> ret;
		if (ManagementWorld->getLocalMapTileObject(tile) != nullptr)
		{
			if (ManagementWorld->getLocalMapTileObject(tile)->getType() == ObjectType::Chest)
			{
				ret = dynamic_cast<ChestObject*>(ManagementWorld->getLocalMapTileObject(tile))->getContain();
			}
		}

		return ret;
	}


	sf::IntRect getLocalMapTileCollisionBox(const sf::Vector2u &tile) const
	{
		if (!sf::Rect<unsigned>(sf::Vector2u(), sf::Vector2u(ManagementWorld->getLocalMapSize(),
			ManagementWorld->getLocalMapSize())).contains(tile))
		{
			return sf::IntRect();
		}
		if (ManagementWorld->getLocalMapTileObject(tile) == nullptr)
		{
			return sf::IntRect();
		}
		if (ObjectsDef->getDefinition(ManagementWorld->getLocalMapTileObjectId(tile))->getCollision())
		{
			sf::IntRect collideObject;
			collideObject.top = tile.y * static_cast<int>(TILE_SIZE);
			collideObject.left = tile.x * static_cast<int>(TILE_SIZE);
			collideObject.height = static_cast<int>(TILE_SIZE);
			collideObject.width = collideObject.height;

			return collideObject;
		}
		return sf::IntRect();

	}


	sf::Vector2f getSpawnPosition() const
	{
		IntegerGenerator<unsigned> Gen;
		sf::Vector2u spawnTile;
		sf::Vector2f spawnPosition;

		while (true)
		{
			spawnTile.x = Gen.get(1, ManagementWorld->getLocalMapSize());
			spawnTile.y = Gen.get(1, ManagementWorld->getLocalMapSize());

			if (ManagementWorld->getLocalMapTileObject(spawnTile) != nullptr)
			{
				if (!ObjectsDef->getDefinition(ManagementWorld->getLocalMapTileObjectId(spawnTile))->getCollision())
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		spawnPosition = (static_cast<sf::Vector2f>(spawnTile) * TILE_SIZE);
		return spawnPosition;
	}
};