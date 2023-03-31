#pragma once

#include <string>
#include "sqllite/sqlite3.h"

class DBHelper
{
public:
	DBHelper();
	~DBHelper();

	void Connect(const char* dbName);
	void ExecuteQuery(const char* sql);

private:
	sqlite3* mDB;
	bool mIsConnected;
};
