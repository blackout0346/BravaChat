#include "DataStorage.h"


SQLite::Database db("chatMessager.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

void DataStorage::createDatabase()
{


	try {
		db.exec("CREATE TABLE Auth(Id INTEGER PRIMARY KEY AUTOINCREMENT     NOT NULL, Login TEXT NOT NULL, NumberPhone INTEGER NOT NULL, Email TEXT NOT NULL , Password TEXT NOT NULL)");
		db.exec("CREATE TABLE User(Id INTEGER PRIMARY KEY AUTOINCREMENT     NOT NULL, LoginId INTEGER NOT NULL, PasswordId INTEGER NOT NULL, PicturePath BLOB NULL,  FOREIGN KEY(LoginId) REFERENCES Auth(Login), FOREIGN KEY(PasswordId) REFERENCES Auth(Password))");
		db.exec("CREATE TABLE Roles(Id INTEGER PRIMARY KEY AUTOINCREMENT    NOT NULL, Name TEXT NOT NULL, UserId INTEGER NOT NULL, FOREIGN KEY(UserId) REFERENCES User(Id))");
		db.exec("CREATE TABLE SignUp(Id INTEGER PRIMARY KEY AUTOINCREMENT   NOT NULL, Name TEXT NOT NULL, Login TEXT NOT NULL, PicturePath  BLOB NULL)");
		db.exec("CREATE TABLE Contact(Id INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, UserId1 INTEGER NOT NULL,UserId2 INTEGER NOT NULL, typeId INTEGER NOT NULL, FOREIGN KEY(UserId1) REFERENCES User(Id), FOREIGN KEY(UserId2) REFERENCES User(Id),  FOREIGN KEY(typeId) REFERENCES Roles(Id) )");
		db.exec("CREATE TABLE Chat(Id INTEGER PRIMARY KEY AUTOINCREMENT     NOT NULL, UserId1 INTEGER NOT NULL, UserId2 INTEGER NOT NULL,typeId INTEGER NOT NULL, FOREIGN KEY(UserId1) REFERENCES User(Id),  FOREIGN KEY(UserId2) REFERENCES User(Id),  FOREIGN KEY(typeId) REFERENCES Roles(Id))");
		db.exec("CREATE TABLE MessageSET(Id INTEGER PRIMARY KEY AUTOINCREMENT, Msg TEXT NOT NULL)");
		db.exec("CREATE TABLE MessageGET(Id INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, UserId INTEGER NOT NULL, SendDate DATETIME DEFAULT CURRENT_TIMESTAMP, chatId INTEGER NOT NULL, MsgId INTEGER NOT NULL, ReplyId INTEGER NULL, FOREIGN KEY(UserId) REFERENCES User(Id), FOREIGN KEY(chatId) REFERENCES chat(Id),  FOREIGN KEY (ReplyId) REFERENCES MessageSET(Id),FOREIGN KEY (MsgId) REFERENCES MessageSET(Id) )");
		db.exec("CREATE TABLE UserChat(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, UserId INTEGER NOT NULL,chatId INTEGER NOT NULL, typeId INTEGER NOT NULL, FOREIGN KEY(UserId) REFERENCES User(Id),  FOREIGN KEY(chatId) REFERENCES Chat(Id),  FOREIGN KEY(typeId) REFERENCES Roles(Id) )");
		cout << "sqlite create database "<< endl;
	}
	catch (exception& e)
	{
		cout << "sqlite " << e.what() << endl;
	}

}

void DataStorage::SelectDatabase(string sqlquery)
{
	try {
		db.exec(sqlquery);
		auto va = db.exec("SELECT * FROM Auth");
		cout << va << endl;
	}
	catch (exception& e)
	{
		cout << "Added Error: " << e.what() << endl;
	}

}


void DataStorage::AddinDatabase(string sqlquery)
{
	try {
		db.exec(sqlquery);
		auto va = db.exec("SELECT * FROM Auth");
		cout << va << endl;
	}
	catch (exception& e)
	{
		cout << "Added Error: " << e.what() << endl;
	}

}

void DataStorage::DeleteDatabase(string sqlquery)
{
	try {
		db.exec(sqlquery);
	}
	catch (exception& e)
	{
		cout << "Delete data " << e.what() << endl;
	}
}

void DataStorage::EditDatabase(string sqlquery)
{
	try {
		db.exec(sqlquery);
	}
	catch (exception& e)
	{
		cout << "Edit data " << e.what() << endl;
	}
}