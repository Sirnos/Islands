#pragma once

#include <string>
#include <sqlite3.h>

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
};