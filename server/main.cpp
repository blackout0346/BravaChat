#include"Route.h"
#include"DataStorage.h"
using namespace std;

int main()
{
	DataStorage db;
    crow::SimpleApp app;
	Route route;
	route.POST();
	route.GET();


}
