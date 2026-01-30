#include "Route.h"


void Route::MessageRoute()
{
    CROW_ROUTE(app, "/message").methods("GET"_method)([=](const crow::request req)
        {
            try {
                crow::json::rvalue x = crow::json::load(req.body);

                cout << x["name"] << endl;
                cout << x["message"] << endl;
                ostringstream  query;
                query << "SELECT M.Id, M.UserId, M.SendDate, M.ChatId, M.MsgId, M.ReplyId FROM MessageGET AS M JOIN User AS U ON U.id = M.UserId JOIN Chat AS C ON C.id = M.ChatId";
                db.SelectQueryDatabase(query.str());
            }
            catch (exception& e)
            {
                cout << "Error /message: " << e.what() << endl;
            }
            return 200;
        });
    CROW_ROUTE(app, "/message/delete").methods("DELETE"_method)([=](const crow::request req)
        {
            try {
                ostringstream query;
                ostringstream query2;
                crow::json::rvalue x = crow::json::load(req.body);
                cout << x["message"] << endl;

                query << "DELETE FROM Message WHERE Id = " << x["message"] << ";";

                db.GetQueryDatabase(query.str());
            }
            catch (exception& e)
            {
                cout << "error /message/delete" << e.what() << endl;
            }

            return 200;
        });
    CROW_ROUTE(app, "/message").methods("GET"_method)([=](const crow::request req)
        {
            try {
                crow::json::rvalue x = crow::json::load(req.body);
                cout << x["message"] << endl;
                ostringstream  query;
                query << "SELECT M.Id, M.UserId, M.SendDate, M.ChatId, M.MsgId, M.ReplyId FROM MessageGET AS M JOIN User AS U ON U.id = M.UserId JOIN Chat AS C ON C.id = M.ChatId";
                db.SelectDatabase(query.str());
            }
            catch (exception& e) {
                cout << "error /message" << e.what() << endl;
            }
            return 200;
        });

    PORT();
}
void Route::ContactsRoute()
{    CROW_ROUTE(app, "/contact").methods("GET"_method)([=](const crow::request req)
        {
            try {
                crow::json::rvalue x = crow::json::load(req.body);

                ostringstream query;
                query << "SELECT C.UserId1,U.Login, C.UserId2 FROM Contact AS CJOIN Users AS U ON U.Id = C.UserId1";
                db.GetQueryDatabase(query.str());
            }
            catch(exception& e)
            {
                cout << "Error /contact: " << e.what() << endl;
            }
            return 200;
        });
}
void Route::UsersRoute()
{
    CROW_ROUTE(app, "/auth").methods("POST"_method)([=](const crow::request& req) {


        crow::json::rvalue x = crow::json::load(req.body);
        try {
            cout << x["Login"] << endl;
            cout << x["numbers"] << endl;
            cout << x["emails"] << endl;
            cout << x["passwords"] << endl;

            ostringstream  query;
            query << "INSERT INTO Users(Login, NumberPhone, Email, Password) VALUES('"
                << x["Login"].s() << "', '"
                << x["numbers"].s() << "','"
                << x["emails"].s() << "','"
                << x["passwords"].s() << "')";
            db.GetQueryDatabase(query.str());
            cout << " I got auth!" << endl;

        }
        catch (exception& e) {
            cout << "error /auth" << e.what() << endl;
        }
        return 200;
        });
    CROW_ROUTE(app, "/login").methods("POST"_method)([&](const crow::request& req) {
        try {

       
        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["name"] << endl;
        cout << x["password"] << endl;
        ostringstream  query;
        query << "SELECT * FROM User WHERE name = " << x["name"].s() << "AND password = " << x["password"] << " OR NumberPhone = " << x["NumberPhone"] << ";";
        db.DeleteDatabase(query.str());
        }
        catch (exception& e) {
            cout << "error /login" << e.what() << endl;
        }
        return 200;
        });
    CROW_ROUTE(app, "/createchat").methods("POST"_method)([=](const crow::request& req) {
        try{
        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["name"] << endl;
        cout << x["users"] << endl;
        cout << " I got chat!" << endl;
        ostringstream query;
        ostringstream query2;
        query2 << "INSERT INTO Roles(Name, UserId) VALUES('" <<x["RoleName"]<<"'"<<"(SELECT Id FROM Users WHERE Id =" << x["Names"] << "))";
        query << "SELECT C.UserId1, C.UserId2, C.typeId FROM Chat AS C JOIN User AS U ON U.Id = C.UserId1 JOIN User AS U ON U.Id = C.UserId2 JOIN Roles AS R ON R.Id = C.typeId " << ";";
        db.AddinDatabase(query.str());
        db.AddinDatabase(query2.str());
        }
        catch (exception& e) {
            cout << "error /login:" << e.what() << endl;
        }
        return crow::response(200, "OK");
        });
    CROW_ROUTE(app, "/users/delete").methods("DELETE"_method)([=](const crow::request req)
        {
            try{
            crow::json::rvalue x = crow::json::load(req.body);

            cout << x["name"] << endl;
            cout << x["message"] << endl;
            ostringstream  query;
            query << "INSERT INTO MessageSET(Msg)VALUES('" << x["message"] << "')";
            db.AddinDatabase(query.str());
            }
            catch (exception& e)
            {
                cout << "error /users/delete" << e.what() << endl;
            }
            return crow::response(200, "complete message");
        });
    PORT();
}
void Route::ChatRoute()
{
    CROW_ROUTE(app, "/users/delete").methods("DELETE"_method)([=](const crow::request req)
        {
            try{
            crow::json::rvalue x = crow::json::load(req.body);
            cout << x["name"] << endl;
            ostringstream  query;
            query << "DELETE FROM User WHERE id = " << x["name"].s() << ";";
            db.DeleteDatabase(query.str());
            }
            catch (exception& e)
            {
                cout << "Error /chat: " << e.what() << endl;
            }

            return 200;
        });
    CROW_ROUTE(app, "/message/delete").methods("DELETE"_method)([=](const crow::request req)
        {
            try {
                ostringstream query;
                ostringstream query2;
                crow::json::rvalue x = crow::json::load(req.body);
                cout << x["message"] << endl;

                query << "DELETE FROM MessageSET WHERE Id = " << x["message"] << ";";
                query2 << "DELETE FROM MessageGET WHERE Id ="<<x["message"] << ";";
                db.DeleteDatabase(query.str());
                db.DeleteDatabase(query2.str());
            }
            catch (exception& e)
            {
                cout << "error /message/delete" << e.what() << endl;
            }

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