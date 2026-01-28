#include "Route.h"

void Route::GET()
{

  /*  CROW_ROUTE(app, "/contact").methods("GET"_method)([=](const crow::request req)
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
        });*/
    CROW_ROUTE(app, "/chat").methods("GET"_method)([=](const crow::request req)
        {
            try {
            crow::json::rvalue x = crow::json::load(req.body);
            cout << x["ChatName"] << endl;
            cout << x["username"] << endl;
            ostringstream  query;
            query << "SELECT UC.chatId, U.Login, TC.typeName FROM UserChat AS UC JOIN Users AS U ON U.Id = UC.UserId JOIN Chats AS C ON C.Id = UC.chatIdJOIN TypeChat AS TC";
            db.SelectQueryDatabase(query.str());
            }
            catch (exception& e)
            {
                cout << "Error /chat: " << e.what() << endl;
            }

            return 200;
        });
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
            query << "INSERT INTO Users(Login, NumberPhone, Email, Password) VALUES('"
                << x["Login"].s() << "', '"
                << x["numbers"].s() << "','"
                << x["emails"].s() << "','"
         /*       << x["Picture"].s() << "','"*/
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
        if (!x)
        {
            return crow::response(400, "invalid json");
        }
        if (!x.has("Login") || !x.has("password"))
        {
            return crow::response(400, "invalid input");
        }
        
        
        ostringstream  query;
        query << "SELECT U.Login, U.NumberPhone, U.Email, U.Password, U.PicturePath FROM Users AS U WHERE U.Login = "<<x["Login"].s() << " AND U.Password = " <<x["password"].s();
        db.SelectQueryDatabase(query.str());
        }
        catch (exception& e) {
            cout << "error /login:" << e.what() << endl;
        }
        return crow::response(200, "OK");
        });
    CROW_ROUTE(app, "/createchat").methods("POST"_method)([=](const crow::request& req) {
        try{
        crow::json::rvalue x = crow::json::load(req.body);
        cout << x["name"] << endl;
        cout << x["users"] << endl;
        cout << " I got chat!" << endl;
        ostringstream query;
        db.GetQueryDatabase(query.str());
        }
        catch (exception& e) {
            cout << "error /createchat" << e.what() << endl;
        }
        return 200;
        });
    CROW_ROUTE(app, "/message").methods("POST"_method)([=](const crow::request req)
        {
            try{
            crow::json::rvalue x = crow::json::load(req.body);
            cout << x["message"] << endl;
            ostringstream  query;
            query << "INSERT INTO Message(Message)VALUES('" << x["message"] << "')";
            db.GetQueryDatabase(query.str());
            }
            catch (exception& e) {
            cout << "error /message" << e.what() << endl;
            }
            return 200;
        });

}
void Route::DELETEV()
{
    CROW_ROUTE(app, "/users/delete").methods("DELETE"_method)([=](const crow::request req)
        {
            try{
            crow::json::rvalue x = crow::json::load(req.body);
            cout << x["Login"] << endl;
            ostringstream  query;
            query << "DELETE FROM User WHERE id = " << x["Login"].s() << ";";
            db.GetQueryDatabase(query.str());
            }
            catch (exception& e)
            {
            cout << "error /users/delete" << e.what() << endl;
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

}
void Route::PORT()
{
    app.port(18080).bindaddr("127.0.0.1")
        .multithreaded()
        .run();
}