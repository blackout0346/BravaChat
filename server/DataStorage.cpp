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

void DataStorage::DeleteContact(int UserId1, int UserId2)
{
	SQLite::Statement query(db, "DELETE FROM Contact WHERE UserId1 = ? AND UserId2 =?");
	query.bind(1, UserId1);
	query.bind(2, UserId2);
	query.exec();
}

void DataStorage::InsertMessage(int userId, int chatId, string Message, int replyId, int ForwardId)
{
	try {
		SQLite::Statement query(db, "INSERT INTO Message(UserId, ChatId,SendDate, Message, ReplyId, ForwardId)VALUES(?, ?, ?, ?, ?, ?)");
		query.bind(1, userId);
		query.bind(2, chatId);
		query.bind(3, getDateTime());
		query.bind(4, Message);
		if (replyId != 0)
		{
			query.bind(5, replyId);
		}
		else {
			query.bind(5, 0);
		}
		if (ForwardId != 0)
		{
			query.bind(6, replyId);
		}
		else {
			query.bind(6, 0);
		}

		query.exec();
	}
	catch (exception& e) {
		std::cout << "SQL Insert Error: " << e.what() << std::endl;
	}
}

crow::json::wvalue DataStorage::GetMessages(crow::json::wvalue msg , int chatId)
{
	std::vector<crow::json::wvalue> rows;
	SQLite::Statement query(db, "SELECT M.Id, M.UserId, U.Login, M.chatId, M.SendDate, M.Message, M.ReplyId, M.ForwardId, M.EditedAt FROM Message AS M INNER JOIN Users AS U ON U.Id = M.UserId WHERE m.chatId = ? ORDER BY SendDate ASC");
	query.bind(1, chatId);
	while (query.executeStep())
	{
		crow::json::wvalue row;
		row["Id"] = query.getColumn(0).getInt();
		row["userId"] = query.getColumn(1).getInt();
		row["Login"] = query.getColumn(2).getText();
		row["chatId"] = query.getColumn(3).getInt();
		row["SendDate"] = query.getColumn(4).getText();
		row["message"] = query.getColumn(5).getText();
	
		if (!query.getColumn(6).isNull())
		{
			row["Replyid"] = query.getColumn(6).getInt();
		}
		if (!query.getColumn(7).isNull())
		{
			row["ForwardId"] = query.getColumn(7).getInt();
		}
		if (!query.getColumn(8).isNull())
		{
			row["EditedAt"] = query.getColumn(8).getText();
		}
		rows.push_back(move(row));
	}
	return crow::json::wvalue(std::move(rows));

}


void DataStorage::InsertChat(int userId, int chatId, string Messages)
{
	SQLite::Statement checkQuery(db, "SELECT C.Id FROM Chats"
		"INNER JOIN UserChat AS UC1 ON C.Id = UC1.chatId"
		"INNER JOIN UserChat AS UC2 ON C.Id = UC2.chatId"
		"WHERE C.isGroup = 0"
		"AND UC1.UserId = ?"
		"AND UC2.UserId = ?"
		"AND(SELECT COUNT(*) FROM UserChat WHERE chatId = C.Id) =2");
}

void DataStorage::InsertContact(int userId1, int userId2)
{
	SQLite::Statement query(db, "INSERT INTO Contact(UserId1, UserId2, createAt) VALUES(?,?,?)");
	query.bind(1, userId1);
	query.bind(2, userId2);
	query.bind(3, getDateTime());
	query.exec();
}

crow::json::wvalue DataStorage::GetContact(int userId, vector<crow::json::wvalue> contacts)
{
	SQLite::Statement query(db, "SELECT U.Id, U.Login FROM Users AS U "
		"INNER JOIN Contact AS C ON (C.UserId2 =U.Id AND C.UserId1 = ?)");
	query.bind(1, userId);
	while (query.executeStep())
	{
		crow::json::wvalue contact;
		contact["Id"] = query.getColumn(0).getInt();
		contact["Login"] = query.getColumn(1).getText();
		contacts.push_back(move(contact));
	}
	return contacts;
}



crow::json::wvalue DataStorage::SelectLogin(string login, string password, int number)
{
	crow::json::wvalue response;
	SQLite::Statement query(db, "SELECT Id, Login, Password FROM Users WHERE Login = ? AND Password = ?");
	query.bind(1, login);
	//query.bind(2, number);
	query.bind(2, password);
	if(query.executeStep())
	{
	
		response["Id"] = query.getColumn(0).getInt();
		response["Login"] = query.getColumn(1).getText();
		response["status"] = "success";

	}
	
	return response;
}

void DataStorage::InsertAuth(string login, string password, string email, int number, string photo )
{
	SQLite::Statement query(db, "INSERT INTO Users(Login, NumberPhone, Email, Password, PicturePath) VALUES(?,?,?,?,?)");
	query.bind(1, login);
	query.bind(2, number);
	query.bind(3, email);
	query.bind(4, password);
	if (!photo.empty())
	{
		query.bind(5, photo);
	}

	query.exec();

}

string DataStorage::getDateTime()
{
	auto date = chrono::system_clock::now();
	auto time = chrono::system_clock::to_time_t(date);
	stringstream ss;
	ss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

crow::json::wvalue DataStorage::setUserId(crow::json::wvalue users, int userId)
{
	SQLite::Statement query(db, "SELECT Id, Login FROM Users WHERE Id = ?" );
	query.bind(1, userId);

	if (query.executeStep())
	{
		users["Id"] = query.getColumn(0).getInt();
		users["Login"] = query.getColumn(1).getText();

	}
	return users;


}

crow::json::wvalue DataStorage::SearchLogin(crow::json::wvalue user, string search)
{
	vector<crow::json::wvalue> users;
	SQLite::Statement query(db, "SELECT Id, Login FROM Users WHERE Login LIKE ? LIMIT 20");
	query.bind(1, "%" + search + "%");

	while (query.executeStep())
	{
		user["Id"] = query.getColumn(0).getInt();
		user["Login"] = query.getColumn(1).getInt();
		users.push_back(move(user));
	}
	return users;
}

