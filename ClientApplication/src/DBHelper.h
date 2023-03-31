#pragma once

#include <string>
#include "sqllite/sqlite3.h"

class DBHelper
{
public:
	//Constructor
	DBHelper();

	// Destructor
	~DBHelper();

	/// <summary>
	/// Connect to database
	/// </summary>
	/// <param name="dbName">database name</param>
	void Connect(const char* dbName);

	/// <summary>
	/// Set player's new high score
	/// </summary>
	/// <param name="playerId">player id</param>
	/// <param name="score">high score</param>
	void SetHighScore(int playerId, int score);

private:
	sqlite3* mDB;
	bool mIsConnected;
};
