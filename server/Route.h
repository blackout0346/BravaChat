#pragma once
#include <string>
#include "crow_all.h"
#include <iostream>
#include "DataStorage.h"
#include"fmt/core.h"
using namespace std;
class Route
{
private:

	crow::SimpleApp app;


public:
	DataStorage db;
	void GET();
	void POST();
	void DELETEV();
	void PORT();

};

