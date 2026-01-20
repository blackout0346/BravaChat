#pragma once
#define SQLITECPP_COMPILE_DLL
#include "SQLiteCpp/SQLiteCpp.h"
#include <iostream>
class DataStorage
{
private:
	SQLite::Database db;
public:

	void createDatabase();
	void AddinDatabase();
};

