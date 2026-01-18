#include "Route.h"

void Route::GET()
{
  /*  CROW_ROUTE(app, "/chat").methods("GET"_method)([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        return x;
        
        });
   */

    //CROW_ROUTE(app, "/Users")([]() 
    // {

    //    return "Antonio";
    //    
    // });
    //app.port(18080).bindaddr("127.0.0.1")
    //    .multithreaded()
    //    .run();

}

void Route::POST()
{
    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        cout << x["Login"].s() << endl;
        cout << x["Password"].s() << endl;
        cout << " I got login!" << endl;
        return 0;
        });
    app.port(18080).bindaddr("127.0.0.1")
        .multithreaded()
        .run();
}

void Route::DELETEV()
{
}
