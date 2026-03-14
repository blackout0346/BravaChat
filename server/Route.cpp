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

                auto result = db.GetMessages(crow::json::wvalue(), chatId);

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
                db.db.exec("BEGIN TRANSACTION");
                SQLite::Statement updateRepliesQuery(db.db,
                    "UPDATE Message SET ReplyId = NULL WHERE replyId =?");
                updateRepliesQuery.bind(1, messageId);
                updateRepliesQuery.exec();

                SQLite::Statement deleteQuery(db.db,
                    "DELETE FROM Message WHERE Id = ?");
                deleteQuery.bind(1, messageId);
                deleteQuery.exec();
                db.db.exec("COMMIT");
                return crow::response(200, "Message deleted");

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
                SQLite::Statement query(db.db,
                    "SELECT M.Id, M.UserId, M.chatId, M.SendDate, U.Login, M.Message FROM Message AS M "
                    "INNER JOIN Users AS U ON M.UserId = U.Id WHERE M.Id = ?");
                query.bind(1, messageId);

                if (query.executeStep())
                {
                    crow::json::wvalue msg;
                    msg["Id"] = query.getColumn(0).getInt();
                    msg["chatId"] = query.getColumn(1).getInt();
                    msg["UserId"] = query.getColumn(2).getInt();
                    msg["sendDate"] = query.getColumn(3).getText();
                    msg["UserName"] = query.getColumn(4).getText();
                    msg["Message"] = query.getColumn(5).getText();

                    return crow::response(200, msg);

                }
                return crow::response(404, "Message not found");


            }
            catch (exception& e)
            {

                return crow::response(400, e.what());

            }
        });
    CROW_ROUTE(app, "/message/<int>/edit").methods("PUT"_method)([&](const crow::request& req, int messageId)
        {
            auto body = crow::json::load(req.body);
            if (!!body || !body.has("message"))
            {
                return crow::response(400, "Missing message text");
            }
            try
            {
                db.editMessage(messageId, body["message"].s());
           
                return crow::response(200, "Message edited");

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
                return crow::response(201, "contact added");
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
                return crow::response(200, "Contact Deleted");

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

            if (!x.has("Login") || !x.has("passwords") || !x.has("emails") || !x.has("numbers")) {
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
            db.InsertAuth(login, Password, email, NumberPhone, photo);
            int userId = static_cast<int>(db.db.getLastInsertRowid());
            crow::json::wvalue response;
            response["Id"] = userId;
            response["message"] = "User registered successfully";
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

            string login = x["Login"].s();
            string Password = x["Password"].s();
            int NumberPhone = x["NumberPhone"].i();

            auto result = db.SelectLogin(login, Password, NumberPhone);

            if (result.keys().empty()) {
                return crow::response(401, "Invalid credentials");
            }
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
            SQLite::Statement query(db.db,
                "SELECT C.Id, C.name, C.isGroup FROM Chats AS C "
                "INNER JOIN UserChat as UC ON UC.chatId = C.Id "
                "WHERE UC.UserId = ?");
            query.bind(1, userId);

            crow::json::wvalue::list chats_list;

            while (query.executeStep())
            {
                crow::json::wvalue chat;
                int chatId = query.getColumn(0).getInt();
                string chatName = query.getColumn(1).getText();
                bool IsGroup = query.getColumn(2).getInt() == 1;

                chat["chatId"] = chatId; 
                chat["IsGroup"] = IsGroup;

                if (IsGroup) {
                    chat["login"] = chatName; 
                }
                else {
                  
                    SQLite::Statement otherUserQuery(db.db,
                        "SELECT Login FROM Users "
                        "WHERE Id = (SELECT UserId FROM UserChat WHERE chatId = ? AND UserId != ? LIMIT 1)");
                    otherUserQuery.bind(1, chatId);
                    otherUserQuery.bind(2, userId);

                    if (otherUserQuery.executeStep()) {
                        chat["login"] = otherUserQuery.getColumn(0).getText();
                    }
                    else {
                        chat["login"] = "Unknown User";
                    }
                }
               
                chats_list.push_back(std::move(chat));
            }

           
            crow::json::wvalue response;
            response["chats"] = std::move(chats_list);
            return crow::response(200, response);
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