#include "Route.h"

void Route::GET()
{

    CROW_ROUTE(app, "/Users").methods("GET"_method)([](const crow::request req)
    {
        crow::json::rvalue x = crow::json::load(req.body);
        
        for (auto i : x)
        {
            i["name"];
        }
        cout << req.body << endl;
        return 200;
    });
    CROW_ROUTE(app, "/Chat").methods("GET"_method)([](const crow::request req)
    {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["ChatName"] << endl;
            cout << x["username"] << endl;
            cout << req.body << endl;
            return 200;
    });
    CROW_ROUTE(app, "/message").methods("GET"_method)([](const crow::request req)
    {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["name"] << endl;
            cout << x["message"] << endl;
            cout << req.body << endl;
            return 200;
    });

    PORT();
}

void Route::POST()
{
    CROW_ROUTE(app, "/auth").methods("POST"_method)([=](const crow::request& req) {
        int id = 1;

        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["Names"] << endl;
        cout << x["emails"] << endl;
        cout << x["passwords"] << endl;
        cout << x["numbers"] << endl;
        string quaery = fmt::format("INSERT INTO Auth(Id, Login, NumberPhone, Email , Password)VALUE({ }, '{ }', '{ }', '{ }', '{ }')", id, x["Names"], x["Login"], x["NumberPhone"], x["Password"]);
        id++;
        db.AddinDatabase(quaery);
        cout << " I got auth!" << endl;
        return 200;
        });
    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req) {
        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["name"] << endl;
        cout << x["password"] << endl;
        cout << " I got login!" << endl;
        return 200;
        });
    CROW_ROUTE(app, "/CreateChat").methods("POST"_method)([](const crow::request& req) {
        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["name"] << endl;
        cout << x["users"] << endl;
        cout << " I got chat!" << endl;
        return 200;
        });
    PORT();
}
void Route::DELETEV()
{
    CROW_ROUTE(app, "/Users").methods("DELETE"_method)([](const crow::request req)
        {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["name"] << endl;
            cout << x["password"] << endl;
            cout << req.body << endl;
            return 200;
        });
    CROW_ROUTE(app, "/message/delete").methods("DELETE"_method)([](const crow::request req)
        {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["message"] << endl;
            cout << req.body << endl;
            return 200;
        });
    PORT();
}
void Route::PORT()
{
    app.port(18080).bindaddr("127.0.0.1")
        .multithreaded()
        .run();
}
