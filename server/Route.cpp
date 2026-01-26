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

            return 200;
    });
    CROW_ROUTE(app, "/message").methods("GET"_method)([=](const crow::request req)
    {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["name"] << endl;
            cout << x["message"] << endl;

            ostringstream  query;
            query << "SELECT M.Id, M.UserId, M.SendDate, M.ChatId, M.Msg, M.ReplyId FROM Message AS M JOIN User AS U ON U.id = M.UserId JOIN Chat AS C ON C.id = M.ChatId";
            db.SelectDatabase(query.str());
            return 200;
    });

    PORT();
}

void Route::POST()
{
    CROW_ROUTE(app, "/auth").methods("POST"_method)([=](const crow::request& req) {


        crow::json::rvalue x = crow::json::load(req.body);
        try {
            cout << x["Login"] << endl;
            cout << x["numbers"] << endl;
            cout << x["emails"] << endl;
            cout << x["passwords"] << endl;

            ostringstream  query;
            query << "INSERT INTO Auth( Login, NumberPhone, Email, Password) VALUES('"
                << x["Login"].s() << "', '"
                << x["numbers"].s() << "','"
                << x["emails"].s() << "','"
                << x["passwords"].s() << "')";
            db.AddinDatabase(query.str());
            cout << " I got auth!" << endl;

        }
        catch (exception& e) {
            cout << e.what() << endl;
        }
        return 200;
        });
    CROW_ROUTE(app, "/login").methods("POST"_method)([=](const crow::request& req) {
        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["name"] << endl;
        cout << x["password"] << endl;
        ostringstream  query;
        query << "SELECT * FROM User WHERE name = " << x["name"].s() << "AND" << "password = " << x["password"] << ";";
        db.DeleteDatabase(query.str());
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
    CROW_ROUTE(app, "/message").methods("POST"_method)([=](const crow::request req)
        {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["name"] << endl;
            cout << x["message"] << endl;
            ostringstream  query;
            query << "INSERT INTO Message( Msg)VALUES(" << x["message"] << ")";
            db.AddinDatabase(query.str());
            return 200;
        });
    PORT();
}
void Route::DELETEV()
{
    CROW_ROUTE(app, "/Users").methods("DELETE"_method)([=](const crow::request req)
        {
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["name"] << endl;
            cout << x["password"] << endl;
            ostringstream  query;
            query << "DELETE FROM User WHERE id = " << x["name"].s() << ";";
            db.DeleteDatabase(query.str());
            cout << req.body << endl;
            return 200;
        });
    CROW_ROUTE(app, "/message/delete").methods("DELETE"_method)([=](const crow::request req)
        {
            ostringstream  query;
            crow::json::rvalue x = crow::json::load(req.body);
            cout << x["message"] << endl;
            query << "DELETE FROM MessageSET Where ";
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
