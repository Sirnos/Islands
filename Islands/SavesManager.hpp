#pragma once

#include <string>
#include <sqlite3.h>

#include "PlayerEntity.hpp"

static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	for (int it = 0; it < argc; it++)
	{
		std::cout << azColName[it] << " " << argv[it] ? argv[it] : "NULL";
		//printf("%s = %s\n", azColName[it], argv[it] ? argv[it] : "NULL");
	}
	std::cout << std::endl;
	//printf("\n");
	return 0;
}

class SavesManager
{
	sqlite3 *characterBase;
	sqlite3 *worldBase;

	std::string pathToSaves = "Data/Saves/";
public:
	SavesManager(int & charBaseErrCode, int & worldBaseErrCode, const std::string & saveName)
	{
		charBaseErrCode = sqlite3_open(std::string(pathToSaves + saveName + '/' + "char.db").data(), &characterBase);
		worldBaseErrCode = sqlite3_open(std::string(pathToSaves + saveName + '/' + "world.db").data(), &characterBase);
	}
	SavesManager()
	{
		sqlite3_close(characterBase);
		sqlite3_close(worldBase);
	}

	void savePlayerStats(const PlayerEntity &player)
	{
		float playerHpMax = player.Stats.HP.getLimit();
		float playerHp = player.Stats.HP.getVar();
		float playerMpMax = player.Stats.MP.getLimit();
		float playerMp = player.Stats.MP.getVar();

		sf::Vector2f playerPos = player.getBody().getPosition();

		char  *error;
		sqlite3_exec(characterBase, "CREATE TABLE  STATS(" \
			"ID  INT  PRIMARY KEY  NOT NULL," \
			"HP_MAX  INT  NOT NULL," \
			"HP  INT  NOT NULL," \
			"MP_MAX  INT  NOT NULL," \
			"MP  INT  NOT NULL," \
			"POS_X  REAL  NOT NULL," \
			"POS_Y  REAL  NOT NULL);",
			sql_callback, NULL, &error);
		ErrorHandler::log(std::string(error));
	}
	void loadPlayerStats()
	{

	}
};