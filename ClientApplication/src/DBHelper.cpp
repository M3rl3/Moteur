#include "DBHelper.h"
#include <stdio.h>

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

}
