#pragma once
#include <string>
#include "crow_all.h"
#include <iostream>
#include "DataStorage.h"
using namespace std;
class Route
{
private:

	crow::SimpleApp app;


public:
	DataStorage db;
	void MessageRoute();
	void ContactsRoute();
	void UsersRoute();
	void ChatRoute();
	void PORT();
	int GetUserIdFromToken(crow::request& req);
};

