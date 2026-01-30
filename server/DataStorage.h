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
	void InsertMessage(int userId, int chatId, string Messages);
	void InsertChat(int userId, int chatId, string Messages);
	void InsertContact(int userId, int chatId, string Messages);
	void InsertUsers(int userId, int chatId, string Messages);
};

