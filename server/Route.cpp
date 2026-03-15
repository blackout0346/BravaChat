#include "Route.h"


void Route::MessageRoute()
{
    CROW_ROUTE(app, "/users/messages").methods("POST"_method)([&](const crow::request req)
        {
            try {
                std::cout << "Raw JSON: " << req.body << std::endl;

                auto x = crow::json::load(req.body);
                if (!x) return crow::response(400, "Invalid JSON");


                if (!x.has("userId") || !x.has("chatId") || !x.has("message")) {
                    return crow::response(400, "fail req");
                }
                int userId = x["userId"].i();
                int chatId = x["chatId"].i();
                int replyId = x["replyId"].i();
                int forwardId = x["forwardId"].i();
                string Message = x["message"].s();

                db.InsertMessage(userId, chatId, Message, replyId, forwardId);
                int messageId = static_cast<int>(db.db.getLastInsertRowid());
                crow::json::wvalue response;
                response["Id"] = messageId;
                response["message"] = "Message sent";
                return crow::response(201, response);

            }
            catch (exception& e)
            {
                return crow::response(400, e.what());
                cout << "Error /message: " << e.what() << endl;
            }

        });
    CROW_ROUTE(app, "/users/messages/chat/<int>")([&](int chatId)
        {
            try {

                auto result = db.SetMessages(crow::json::wvalue(), chatId);

                return crow::response(200, result);

            }
            catch (exception& e)
            {
                return crow::response(400, e.what());
                cout << "Error /users/messages/chat/<int>: " << e.what() << endl;
            }

        });
    CROW_ROUTE(app, "/message/<int>/delete").methods("DELETE"_method)([&](int messageId)
        {
            try {


                db.DeleteMessage(messageId);
                crow::json::wvalue res;
                res["status"] = "Message Deleted";
                return crow::response(200, res);

            }
            catch (exception& e)
            {
                db.db.exec("ROLLBACK");
                return crow::response(400, e.what());

            }
        });
    CROW_ROUTE(app, "/message/<int>")([&](int messageId)
        {
            try {

                auto result = db.GetMessages(messageId);

                return crow::response(200, result);


            }
            catch (exception& e)
            {

                return crow::response(400, e.what());

            }
        });
    CROW_ROUTE(app, "/message/<int>/edit").methods("PUT"_method)([&](const crow::request& req, int messageId)
        {
            auto body = crow::json::load(req.body);
            cout <<"Raw JSON" << body << endl;
            if (!body.has("message"))
            {
                return crow::response(400, "Missing message text");
            }
            try
            {
                db.editMessage(messageId, body["message"].s());
                crow::json::wvalue res;
                res["status"] = "edited";
                return crow::response(200, res);

            }
            catch (exception& e)
            {

                return crow::response(400, e.what());

            }
        });
}
void Route::ContactsRoute()
{
    CROW_ROUTE(app, "/contacts").methods("GET"_method)([&](const crow::request req)
        {
            try {
                crow::json::rvalue x = crow::json::load(req.body);
                if (!x.has("UserId1") || !x.has("UserId2"))
                {
                    return crow::response(400, "missing required fields! ");
                }
                int user1 = x["UserId1"].i();
                int user2 = x["UserId2"].i();
                if (user1 == user2)
                {
                    return crow::response(400, "cannot add yourself as contact! ");
                }
                db.InsertContact(user1, user2);
                crow::json::wvalue res;
                res["status"] = "added";
                return crow::response(201, res);
            }
            catch (exception& e)
            {
                cout << "Error /contact: " << e.what() << endl;

            }

        });
    CROW_ROUTE(app, "/contacts/<int>")([&](int userId)
        {
            try {

       
                auto result = db.GetContact(userId);
                return crow::response(200, result);

            }
            catch (exception& e)
            {
                return crow::response(400, e.what());
            }

        });
    CROW_ROUTE(app, "/contacts/<int>/<int>").methods("DELETE"_method)([&](int userId1, int userId2)
        {
            try {
                if (userId1 > userId2)
                {
                    swap(userId1, userId2);
                }
                db.DeleteContact(userId1, userId2);
                crow::json::wvalue res;
                res["status"] = "Contact Deleted";
                return crow::response(200, res);

            }
            catch (exception& e)
            {
                return crow::response(400, e.what());
            }

        });
    CROW_ROUTE(app, "/contacts/add").methods("POST"_method)([&](const crow::request& req)
        {
            try {
                auto x = crow::json::load(req.body);
                std::cout << "Raw JSON: " << req.body << std::endl;
                if (!x) {
                    return crow::response(400);
                }
                if (!x.has("userId1") || !x.has("userId2"))
                {
                    return crow::response(400, "missing keys: userId1 or userId2");
                }
                int u1 = x["userId1"].i();
                int u2 = x["userId2"].i();
                db.InsertContact(u1, u2);
                return crow::response(200, "Contact Added");

            }
            catch (exception& e)
            {
                return crow::response(400, e.what());
            }

        });

}
void Route::UsersRoute()
{
    CROW_ROUTE(app, "/auth").methods("POST"_method)([&](const crow::request& req) {


        auto x = crow::json::load(req.body);
        cout<<"raw json" << req.body << endl;
        if (!x) return crow::response(400, "Invalid JSON format");

        try {
       
            if (!x.has("Login") || !x.has("passwords") || !x.has("emails") || !x.has("numbers"))
    {
                return crow::response(400, "Missing required fields: Login, passwords, emails or numbers");
            }

            string login = x["Login"].s();
            string Password = x["passwords"].s();
            string email = x["emails"].s();
            string NumberPhone = x["numbers"].s();
            string photo = "";
            if (x.has("photo")) {
                photo = x["photo"].s();
            }
        
            string token = GT.generateToken();
            db.InsertAuth(login, Password, email, NumberPhone, photo, token);
            int userId = static_cast<int>(db.db.getLastInsertRowid());
            
            crow::json::wvalue response;
            response["Id"] = userId;
            response["Token"] = token;
          
            return crow::response(201, response);


        }
        catch (exception& e) {
            crow::json::wvalue error_res;
            error_res["error"] = e.what();
            return crow::response(400, error_res);
        }
        });
    CROW_ROUTE(app, "/login").methods("POST"_method)([&](const crow::request& req) {
        try
        {
            crow::json::rvalue x = crow::json::load(req.body);
            string token = GT.generateToken();
            string login = x["Login"].s();
            string Password = x["Password"].s();
      
            string NumberPhone = x["NumberPhone"].s();
            auto result = db.SelectLogin(login, Password, NumberPhone);
            if (result.keys().empty())
            {
                return crow::response(401, "Invalid credentials");
            }
     
            int userId = stoi(result["Id"].dump());

         
            db.UpdateUserToken(userId, token);
            result["Token"] = token;
            return crow::response(200, result);

        }
        catch (exception& e) {
            return crow::response(400, e.what());
        }

        });
    CROW_ROUTE(app, "/users/<int>")([&](int userId) {
        try
        {
            const crow::json::wvalue user;
            auto result = db.setUserId(user, userId);
            return crow::response(200, result);
        }
        catch (exception& e) {
            return crow::response(400, "invalid login");
        }

        });
    CROW_ROUTE(app, "/users/search/<string>")([&](string searchTerm) {
        try
        {

       
            auto result = db.SearchLogin(searchTerm);
            return crow::response(200, result);
        }
        catch (exception& e) {
            return crow::response(400, "invalid login");
        }

    });



}
void Route::ChatRoute()
{
    CROW_ROUTE(app, "/chats").methods("POST"_method)([&](const crow::request req)
        {
            bool transactionStarted = false;
            try {
                crow::json::rvalue x = crow::json::load(req.body);
                cout<<"chats rew:" << x << endl;
                if (!x || !x.has("name") || !x.has("UserIds"))
                {
                    return crow::response(400, "missing required fields");
                }
                string groupName = x["name"].s();
                vector<int> userIds;
                for (auto& id : x["UserIds"]) 
                {
                    userIds.push_back(id.i());
                }
                db.CreateGroup(groupName, userIds);
                crow::json::wvalue response;
                response["message"] = "Group created successfully";
                return crow::response(201, response);

            }catch(const exception& e)
            {
         
                return crow::response(400, e.what());
            }
        });
    CROW_ROUTE(app, "/chats/user/<int>")([&](int userId) {
        try {
            auto result= db.GetChatUser(userId);

            return crow::response(200, result);
        }
        catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return crow::response(400, e.what());
        }
        });
}

void Route::PORT()
{
    app.port(18080).bindaddr("127.0.0.1")
        .multithreaded()
        .run();
}