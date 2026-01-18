#pragma once
#include <string>
#include "crow.h"
#include <iostream>


using namespace std;
class Route
{
private:
	crow::SimpleApp app;
	string name;
	string email;
	string login;
public:
	void GET();
	void POST();
	void DELETEV();


};

