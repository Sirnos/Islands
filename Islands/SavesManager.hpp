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
	if (argc == 7)
	{
		auto getFloatRowParam = [argv](int n_argv)->float
		{
			return std::stof(std::string(argv[n_argv]));
		};

		float HpMax = getFloatRowParam(1);
		float Hp = getFloatRowParam(2);
		float MpMax = getFloatRowParam(3);
		float Mp = getFloatRowParam(4);
		sf::Vector2f Position(getFloatRowParam(5), getFloatRowParam(6));

		auto ptrToPlayer = static_cast<PlayerEntity*>(Player);
		if (ptrToPlayer != nullptr)
		{
			ptrToPlayer->Stats.operator=(EntityStats(HpMax, MpMax, DEFAULT_PLAYER_SPEED, Hp, Mp));
			ptrToPlayer->setPosition(Position);
		}
	}
	return 0;
}

//static int get_player_inventory_callback(void *Inventory, int argc, char **argv, char **ColName) {}

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
			ErrorHandler::log(std::string(error));
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
			"ID  INT  PRIMARY KEY  NOT NULL," \
			"HP_MAX  REAL  NOT NULL," \
			"HP  REAL  NOT NULL," \
			"MP_MAX  REAL  NOT NULL," \
			"MP  REAL  NOT NULL," \
			"POS_X  REAL  NOT NULL," \
			"POS_Y  REAL  NOT NULL);");
		SqlQuery(playerDBase, "DELETE FROM STATS");

		std::string insertPlayerStatsQueryStr = "INSERT INTO STATS VALUES(1, ";
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
	void savePlayerInventory(const std::vector<ItemField> &playerInv)
	{
		if (!isValid()) { return; }

		SqlQuery(playerDBase, 
			"CREATE TABLE IF NOT EXISTS INVENTORY(" \
			"ID  INT  NOT NULL," \
			"AMOUNT  INT  NOT NULL);");
		SqlQuery(playerDBase, "DELETE FROM INVENTORY");

		const std::string insertQueryBegin = "INSERT INTO INVENTORY VALUES(";
		std::string insertQuery = insertQueryBegin;

		for (const auto & it : playerInv)
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
	//void loadPlayerInventory(){}
};