#pragma once

#include <string>
#include <sqlite3.h>

#include "Directories.hpp"
#include "PlayerEntity.hpp"

const std::string playerDBaseName{ "player.db" };
const std::string worldDBaseName{ "world.db" };

static int empty_sql_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	return 0;
}

static int get_player_stats_callback(void *Player, int argc, char **argv, char **ColName)
{
	if (argc == 6)
	{
		auto getFloatRowParam = [argv](int n_argv)->float
		{
			return std::stof(std::string(argv[n_argv]));
		};

		float HpMax = getFloatRowParam(0);
		float Hp = getFloatRowParam(1);
		float MpMax = getFloatRowParam(2);
		float Mp = getFloatRowParam(3);
		sf::Vector2f Position(getFloatRowParam(4), getFloatRowParam(5));

		auto ptrToPlayer = static_cast<PlayerEntity*>(Player);
		if (ptrToPlayer != nullptr)
		{
			ptrToPlayer->Stats.operator=(EntityStats(HpMax, MpMax, DEFAULT_PLAYER_SPEED, Hp, Mp));
			ptrToPlayer->setPosition(Position);
		}
	}
	return 0;
}

static int get_player_inventory_callback(void *vectorInventory, int argc, char **argv, char **ColName) 
{
	auto getIntParam = [argv](int n_argv)->unsigned
	{
		return static_cast<unsigned>(std::stoi(std::string(argv[n_argv])));
	};

	if (argc == 2)
	{
		auto ptrToVector = static_cast<std::vector<ItemField>*>(vectorInventory);
		if (ptrToVector != nullptr)
		{
			ptrToVector->push_back(ItemField{ getIntParam(0), getIntParam(1) });
		}
	}
	return 0;
}

static int get_row_count_callback(void *ptrToInt, int argc, char **argv, char **ColName)
{
	if (ptrToInt != nullptr)
	{
		if (argc == 1)
		{
			*(static_cast<int*>(ptrToInt)) = std::stoi(argv[0]);
		}
	}

	return 0;
}

class SavesManager
{
	sqlite3 *playerDBase;
	sqlite3 *worldDBase;

	bool valid = true;


	void SqlQuery(sqlite3 *Dbase, const std::string &query, void *Data = nullptr, sqlite3_callback callback = empty_sql_callback)
	{
		char *error;
		sqlite3_exec(Dbase, query.data(), callback, Data, &error);
		if (error != nullptr)
		{
			ErrorHandler::logToFile(std::string(error));
			sqlite3_free(error);
		}
	}
public:
	SavesManager() = delete;
	SavesManager(const SavesManager &other) = delete;
	SavesManager(const std::string & saveName)
	{
		namespace fs = boost::filesystem;
		fs::path currentSaveDirPath(fs::current_path());
		currentSaveDirPath += SAVES_DIR;
		currentSaveDirPath += saveName;

		auto buildPathToFile = [currentSaveDirPath](const std::string &file) -> fs::path
		{
			fs::path toBuild = currentSaveDirPath;
			toBuild += SLASH;
			toBuild += file;

			return toBuild;
		};
		fs::path pathToPlayerDBase{ buildPathToFile(playerDBaseName) };
		fs::path pathToWorldDBase{ buildPathToFile(worldDBaseName) };

		if (!fs::exists(currentSaveDirPath))
		{
			if (!fs::create_directory(currentSaveDirPath))
			{
				valid = false;
			}		
		}

		sqlite3_open(pathToPlayerDBase.string().data(), &playerDBase);
		ErrorHandler::logToFile(sqlite3_errmsg(playerDBase));
		if (!fs::exists(pathToPlayerDBase))
		{
			valid = false;
		}

		sqlite3_open(pathToWorldDBase.string().data(), &worldDBase);
		ErrorHandler::logToFile(sqlite3_errmsg(worldDBase));
		if (!fs::exists(pathToPlayerDBase))
		{
			valid = false;
		}
	}
	~SavesManager()
	{
		sqlite3_close(playerDBase);
		sqlite3_close(worldDBase);
	}


	bool isValid() const
	{
		return valid;
	}


	void savePlayerStats(const PlayerEntity &player)
	{
		if (!isValid()) { return; }

		std::string playerHpMax = std::to_string(player.Stats.HP.getLimit());
		std::string playerHp = std::to_string(player.Stats.HP.getVar());
		std::string playerMpMax = std::to_string(player.Stats.MP.getLimit());
		std::string playerMp = std::to_string(player.Stats.MP.getVar());

		sf::Vector2f playerPos = player.getBody().getPosition();
		std::string playerPosStr = std::to_string(playerPos.x);
		playerPosStr += (", " + std::to_string(playerPos.y));

		SqlQuery(playerDBase,
			"CREATE TABLE IF NOT EXISTS STATS(" \
			"HP_MAX  REAL  NOT NULL," \
			"HP  REAL  NOT NULL," \
			"MP_MAX  REAL  NOT NULL," \
			"MP  REAL  NOT NULL," \
			"POS_X  REAL  NOT NULL," \
			"POS_Y  REAL  NOT NULL);");
		SqlQuery(playerDBase, "DELETE FROM STATS");

		std::string insertPlayerStatsQueryStr = "INSERT INTO STATS VALUES(";
		auto concat = [&insertPlayerStatsQueryStr](const std::string &value)
		{
			insertPlayerStatsQueryStr += (value + ", ");
		};

		concat(playerHpMax);
		concat(playerHp);
		concat(playerMpMax);
		concat(playerMp);
		insertPlayerStatsQueryStr += (playerPosStr + ");");

		SqlQuery(playerDBase, insertPlayerStatsQueryStr.data());
	}
	void savePlayerInventory(const PlayerInventory &playerInv)
	{
		if (!isValid()) { return; }

		SqlQuery(playerDBase, 
			"CREATE TABLE IF NOT EXISTS INVENTORY(" \
			"ID  INT  NOT NULL," \
			"AMOUNT  INT  NOT NULL);");
		SqlQuery(playerDBase, "DELETE FROM INVENTORY");

		const std::string insertQueryBegin = "INSERT INTO INVENTORY VALUES(";
		std::string insertQuery = insertQueryBegin;

		std::vector<ItemField> inventory;

		for (auto & mainInvRow : playerInv.getMainInventory())
		{
			inventory.insert(inventory.end(), mainInvRow.begin(), mainInvRow.end());
		}
		inventory.insert(inventory.end(), playerInv.getArmorInventory().begin(), playerInv.getArmorInventory().end());
		inventory.insert(inventory.end(), playerInv.getHandInventory().begin(), playerInv.getHandInventory().end());
		inventory.push_back(playerInv.getHoldItem());


		for (const auto & it : inventory)
		{
			insertQuery += (std::to_string(it.ItemId) + ", " + std::to_string(it.ItemAmount) + ");");
			SqlQuery(playerDBase, insertQuery);
			insertQuery = insertQueryBegin;
		}
	}


	void loadPlayerStats(PlayerEntity &player)
	{
		if (!isValid()) { return; }

		std::string query = "SELECT * FROM STATS;";
		SqlQuery(playerDBase, query, &player, get_player_stats_callback);
	}
	void loadPlayerInventory(PlayerInventory &playerInv)
	{
		std::vector<ItemField> inventoryBuffer;
		SqlQuery(playerDBase, "SELECT * FROM INVENTORY;", &inventoryBuffer, get_player_inventory_callback);

		if (!inventoryBuffer.empty())
		{
			size_t bufferOffset = 0;

			sf::Vector2u mainInvIterator;
			for (mainInvIterator.x = 0; mainInvIterator.x < PLAYER_INVENTORY_SIZE; mainInvIterator.x++)
			{
				for (mainInvIterator.y = 0; mainInvIterator.y < PLAYER_INVENTORY_SIZE; mainInvIterator.y++)
				{
					playerInv.setInventoryField(mainInvIterator, inventoryBuffer[bufferOffset]);
					bufferOffset++;
				}
			}

			for (size_t armorInvIt = 0; armorInvIt < PLAYER_ARMOR_INVENTORY_SIZE; armorInvIt++)
			{
				playerInv.setArmorField(armorInvIt, inventoryBuffer[bufferOffset]);
				bufferOffset++;
			}

			for (size_t beltInvIt = 0; beltInvIt < PLAYER_INVENTORY_SIZE; beltInvIt++)
			{
				playerInv.setHandInventoryField(beltInvIt, inventoryBuffer[bufferOffset]);
				bufferOffset++;
			}
			playerInv.setHoldItem(inventoryBuffer[bufferOffset]);
		}
	}


	void saveLocalMap(const std::vector<std::vector<MapTile>> &map)
	{
		SqlQuery(worldDBase, "CREATE TABLE IF NOT EXISTS TERRAIN(VAL  BLOB  NOT NULL);");
		SqlQuery(worldDBase, "CREATE TABLE IF NOT EXISTS OBJECTS(VAL  BLOB  NOT NULL);");

		SqlQuery(worldDBase, "DELETE FROM TERRAIN");
		SqlQuery(worldDBase, "DELETE FROM OBJECTS");

		std::vector<TerrainType> terrainRow;
		std::vector<int> objectsRow;

		sqlite3_stmt *terrainRes, *objectsRes;
		std::string insertTerrainQuery = "INSERT INTO TERRAIN VALUES( ? );";
		std::string insertObjectsQuery = "INSERT INTO OBJECTS VALUES( ? );";

		for (auto & row : map)
		{
			sqlite3_prepare_v2(worldDBase, insertTerrainQuery.data(), insertTerrainQuery.size() + 1, &terrainRes, nullptr);
			sqlite3_prepare_v2(worldDBase, insertObjectsQuery.data(), insertObjectsQuery.size() + 1, &objectsRes, nullptr);
			

			for (auto & cell : row)
			{
				int objectId = 0;
				if (cell.TileObject != nullptr)
				{
					objectId = cell.TileObject->getId();
				}

				objectsRow.push_back(objectId);
				terrainRow.push_back(cell.Terrain);
			}
			terrainRow.shrink_to_fit();
			objectsRow.shrink_to_fit();

			int errCode = sqlite3_bind_blob(terrainRes, 1, terrainRow.data(), sizeof(TerrainType) * terrainRow.size(), SQLITE_STATIC);
			sqlite3_step(terrainRes);
			sqlite3_finalize(terrainRes);

			int errCode2 = sqlite3_bind_blob(objectsRes, 1, objectsRow.data(), sizeof(int) * objectsRow.size(), SQLITE_STATIC);
			sqlite3_step(objectsRes);
			sqlite3_finalize(objectsRes);


			terrainRow.clear();
			objectsRow.clear();
		}
	}


	std::vector<std::vector<TerrainType>> loadMapTerrain()
	{
		sqlite3_blob *terrainRow;
		std::vector<std::vector<TerrainType>> ret;

		
		int terrainRows = 0;
		SqlQuery(worldDBase, "SELECT Count(*) FROM TERRAIN", &terrainRows, get_row_count_callback);
		if (terrainRows != 0)
		{
			for (size_t row = 1; row < terrainRows + 1; row++)
			{
				int results = sqlite3_blob_open(worldDBase, "main", "TERRAIN", "VAL", row, NULL, &terrainRow);
				if (results == SQLITE_OK)
				{
					int rowSize = sqlite3_blob_bytes(terrainRow) / sizeof(TerrainType);
					ret.push_back(std::vector<TerrainType>(rowSize));

					sqlite3_blob_read(terrainRow, ret.back().data(), rowSize * sizeof(TerrainType), 0);
				}
				sqlite3_blob_close(terrainRow);
			}
		}
		return ret;
	}
};