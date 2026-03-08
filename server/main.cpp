#include"Route.h"
#include"DataStorage.h"
using namespace std;

int main()
{
	DataStorage db;
	Route route;
	db.createDatabase();

	route.UsersRoute();
	route.ChatRoute();
	route.ContactsRoute();
	route.MessageRoute();
	route.PORT();
}
