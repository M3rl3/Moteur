#include "DBHelper.h"
#include <stdio.h>

std::string CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS player(id INTEGER PRIMARY KEY, high_score INTEGER NOT NULL)";

DBHelper::DBHelper() 
	: mDB(nullptr)
	, mIsConnected(false)
{
}

DBHelper::~DBHelper()
{
	if (mDB)
	{
		printf("Closing database  connection ...\n");
		sqlite3_close(mDB);
	}
}

void DBHelper::Connect(const char* dbName)
{
	if (mIsConnected) {
		printf("Already connected to a DB\n");
		return;
	}

	int result = sqlite3_open(dbName, &mDB);
	if (result != SQLITE_OK) {
		printf("Failed to establish connection to '%s' with error code: %d\n", dbName, result);
		return;
	}

	printf("Successful connection to database: '%s' with code: %d\n", dbName, result);
	mIsConnected = true;

	// Create table player if not exist
	result = sqlite3_exec(mDB, CREATE_TABLE_SQL.c_str(), NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		printf("Failed to create table player due to error code: %d\n", result);
		return;
	}
}

void DBHelper::SetHighScore(int playerId, int score)
{
	sqlite3_stmt* stmt;
	std::string query = "INSERT OR REPLACE INTO player(id, high_score) VALUES(?, ?)";
	sqlite3_prepare_v2(mDB, query.c_str(), -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, playerId);
	sqlite3_bind_int(stmt, 2, score);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

int DBHelper::GetHighScore(int playerId)
{
	sqlite3_stmt* stmt;
	std::string query = "SELECT high_score FROM player WHERE id = ?";
	sqlite3_prepare_v2(mDB, query.c_str(), -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, playerId);
	int score = -1;

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		score = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);

	return score;
}
