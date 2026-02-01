#include"Route.h"
#include"DataStorage.h"
using namespace std;

int main()
{
	DataStorage db;
    crow::SimpleApp app;
	Route route;
	db.createDatabase();

	route.UsersRoute();
	route.ChatRoute();
	route.ContactsRoute();
	route.MessageRoute();
	route.PORT();
}
