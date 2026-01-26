#include"Route.h"
#include"DataStorage.h"
using namespace std;

int main()
{
	DataStorage db;
    crow::SimpleApp app;
	Route route;
	db.createDatabase();
	route.POST();
	route.GET();


}
