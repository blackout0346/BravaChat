#include "DataStorage.h"


SQLite::Database db("chatMessager.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

void DataStorage::createDatabase()
{

	try {
		db.exec("CREATE TABLE Auth(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Login TEXT NOT NULL, NumberPhone INTEGER NOT NULL, Email TEXT NOT NULL , Password TEXT NOT NULL)");
		db.exec("CREATE TABLE User(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, LoginId TEXT NOT NULL, PasswordId TEXT NOT NULL,  FOREIGN KEY(LoginId) REFERENCES Auth(Login), FOREIGN KEY(PasswordId) REFERENCES Auth(Password)");
		db.exec("CREATE TABLE Roles(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Name TEXT NOT NULL, UserId INTEGER NOT NULL, FOREIGN KEY(UserId) REFERENCES User(Id))");
		db.exec("CREATE TABLE SignUp(Id INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, Name TEXT NOT NULL, Login TEXT NOT NULL, PicturePath TEXT NOT NULL)");
		db.exec("CREATE TABLE Contact(Id INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, UserId1 INTEGER NOT NULL,UserId2 INTEGER NOT NULL, typeId INTEGER NOT NULL, FOREIGN KEY(UserId1) REFERENCES User(Id), FOREIGN KEY(UserId2) REFERENCES User(Id),  FOREIGN KEY(typeId) REFERENCES Roles(Id) )");
		db.exec("CREATE TABLE Chat(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, UserId1 INTEGER NOT NULL, UserId2 INTEGER NOT NULL,typeId INTEGER NOT NULL, FOREIGN KEY(UserId1) REFERENCES User(Id),  FOREIGN KEY(UserId2) REFERENCES User(Id),  FOREIGN KEY(typeId) REFERENCES Roles(Id))");
		db.exec("CREATE TABLE Message(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, UserId INTEGER NOT NULL , SendDate DATE NOT NULL, chatId INTEGER NOT NULL, Msg TEXT NOT NULL, ReplyId INTEGER NOT NULL, FOREIGN KEY(UserId) REFERENCES User(Id), FOREIGN KEY(chatId) REFERENCES chat(Id),  FOREIGN KEY (ReplyId) REFERENCES Message(Id) )");
		db.exec("CREATE TABLE UserChat(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, UserId INTEGER NOT NULL,chatId INTEGER NOT NULL, typeId INTEGER NOT NULL, FOREIGN KEY(UserId) REFERENCES User(Id),  FOREIGN KEY(chatId) REFERENCES Chat(Id),  FOREIGN KEY(typeId) REFERENCES Roles(Id) )");
	}
	catch (exception& e)
	{
		cout << "sqlite " << e.what() << endl;
	}

}

void DataStorage::AddinDatabase(string sqlquery)
{
	try {
		db.exec(sqlquery);
		db.exec("SELECT * FROM Auth");

	}
	catch(exception& e)
	{
		cout << "Added data " << e.what() << endl;
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
