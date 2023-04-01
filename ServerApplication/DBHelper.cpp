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

std::map<int32_t, int32_t> DBHelper::GetTopHighScores()
{
	std::map<int32_t, int32_t> highScores;
	sqlite3_stmt* statement;

	std::string query = "SELECT id, high_score FROM player ORDER BY high_score DESC LIMIT 20";
	
	sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, NULL);

	while (sqlite3_step(statement) == SQLITE_ROW) {
		int32_t playerId = sqlite3_column_int(statement, 0);
		int32_t score = sqlite3_column_int(statement, 1);
		
		highScores[playerId] = score;
	}

	sqlite3_finalize(statement);

	return highScores;
}
