#pragma once
#define SQLITECPP_COMPILE_DLL
#define _CRT_SECURE_NO_WARNINGS
#include "SQLiteCpp/SQLiteCpp.h"
#include <iostream>
#include <string>
#include"crow_all.h"
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
	void DeleteContact(int UserId1, int UserId2);
	void InsertMessage(int userId, int chatId, string Message, int replyId, int ForwardId);
	crow::json::wvalue GetMessages(crow::json::wvalue  msg, int chatId);
	void InsertChat(int userId, int chatId, string Messages);
	void InsertContact(int userId1, int userId2);
	crow::json::wvalue GetContact(int userId);
	crow::json::wvalue SelectLogin(string login, string password, int number);
	void InsertAuth(string login, string password, string email, int number, string photo);
	void AddContact(int u1, int u2);
	void editMessage(int  messageId, string message);
	string getDateTime();
	crow::json::wvalue setUserId(crow::json::wvalue user,int userId);
	crow::json::wvalue SearchLogin( string search);
};

