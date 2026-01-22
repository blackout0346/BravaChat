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
	DataStorage();
	void createDatabase();
	void AddinDatabase(string sqlquery);
	void DeleteDatabase(string sqlquery);
	void EditDatabase(string sqlquery);
};

