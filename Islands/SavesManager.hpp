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

class SavesManager
{
	sqlite3 *playerDBase;
	sqlite3 *worldDBase;

	bool valid = true;
public:
	SavesManager() = delete;
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

		float playerHpMax = player.Stats.HP.getLimit();
		float playerHp = player.Stats.HP.getVar();
		float playerMpMax = player.Stats.MP.getLimit();
		float playerMp = player.Stats.MP.getVar();

		sf::Vector2f playerPos = player.getBody().getPosition();

		char  *error;
		sqlite3_exec(playerDBase, "CREATE TABLE  STATS(" \
			"ID  INT  PRIMARY KEY  NOT NULL," \
			"HP_MAX  INT  NOT NULL," \
			"HP  INT  NOT NULL," \
			"MP_MAX  INT  NOT NULL," \
			"MP  INT  NOT NULL," \
			"POS_X  REAL  NOT NULL," \
			"POS_Y  REAL  NOT NULL);",
			empty_sql_callback, NULL, &error);
		if (error != nullptr)
		{
			ErrorHandler::log(std::string(error));
		}
	}
	//void loadPlayerStats(){}
};