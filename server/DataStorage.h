#pragma once
#define SQLITECPP_COMPILE_DLL
#include "SQLiteCpp/SQLiteCpp.h"
#include <iostream>
#include <string>

using namespace std;
class DataStorage
{
private:

public:
	SQLite::Database db;
	DataStorage() : db("chatMessager.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {};
	void createDatabase();
	void SelectQueryDatabase(string sqlquery);
	void GetQueryDatabase(string sqlquery);
	
};

