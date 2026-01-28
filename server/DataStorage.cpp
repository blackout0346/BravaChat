#include "DataStorage.h"


SQLite::Database db("chatMessager.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

void DataStorage::createDatabase()
{

	try {
		db.exec("CREATE TABLE Chats (Id	INTEGER,typeId	INTEGER NOT NULL,PRIMARY KEY(Id AUTOINCREMENT),FOREIGN KEY(typeId) REFERENCES TypeChat(Id)); ");
		db.exec("CREATE TABLE Users (Id	INTEGER NOT NULL,Login	TEXT NOT NULL,NumberPhone	INTEGER NOT NULL,Email	TEXT NOT NULL,Password	TEXT NOT NULL,PicturePath	BLOB,PRIMARY KEY(Id AUTOINCREMENT)); ");
		db.exec("CREATE TABLE Roles(Id	INTEGER NOT NULL,Name	TEXT NOT NULL,PRIMARY KEY(Id AUTOINCREMENT)); ");	
		db.exec("CREATE TABLE Contact (Id	INTEGER NOT NULL,UserId1	INTEGER NOT NULL,UserId2	INTEGER NOT NULL,PRIMARY KEY(Id AUTOINCREMENT),FOREIGN KEY(UserId1) REFERENCES Users(Id),FOREIGN KEY(UserId2) REFERENCES Users(Id)); ");
		db.exec("CREATE TABLE TypeChat (Id	INTEGER,typeName	TEXT NOT NULL,PRIMARY KEY(Id AUTOINCREMENT)); ");
		db.exec("CREATE TABLE GroupChat (Id	INTEGER NOT NULL,UserId	INTEGER NOT NULL,typeId	INTEGER NOT NULL,NameChat	TEXT NOT NULL,PRIMARY KEY(Id AUTOINCREMENT),FOREIGN KEY(typeId) REFERENCES Roles(Id)); ");
		db.exec("CREATE TABLE Message (Id	INTEGER NOT NULL,UserId	INTEGER NOT NULL,SendDate	DATETIME DEFAULT CURRENT_TIMESTAMP,chatId	INTEGER NOT NULL,Message	TEXT NOT NULL,ReplyId	INTEGER,PRIMARY KEY(Id AUTOINCREMENT),FOREIGN KEY(ReplyId) REFERENCES Message(Id),FOREIGN KEY(UserId) REFERENCES Users(Id),FOREIGN KEY(chatId) REFERENCES Chats(Id)); ");
		db.exec("CREATE TABLE UserChat (Id	INTEGER NOT NULL,UserId	INTEGER NOT NULL,chatId	INTEGER NOT NULL,PRIMARY KEY(Id AUTOINCREMENT),FOREIGN KEY(UserId) REFERENCES Users(Id),FOREIGN KEY(chatId) REFERENCES Chats(Id)); ");
		cout << "sqlite create database "<< endl;
	}
	catch (exception& e)
	{
		cout << "sqlite " << e.what() << endl;
	}

}

void DataStorage::SelectQueryDatabase(string sqlquery)
{
	try {
		db.execAndGet(sqlquery);
	}
	catch (exception& e)
	{
		cout << "select data " << e.what() << endl;
	}
}


void DataStorage::GetQueryDatabase(string sqlquery)
{
	try {
		db.exec(sqlquery);
	}
	catch (exception& e)
	{
		cout << "get data " << e.what() << endl;
	}
}