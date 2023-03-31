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

	/// <summary>
	/// Returns the high score of the provided playerId
	/// </summary>
	/// <param name="playerId">The player id</param>
	/// <returns>The score</returns>
	int GetHighScore(int playerId);

private:
	sqlite3* mDB;
	bool mIsConnected;
};
