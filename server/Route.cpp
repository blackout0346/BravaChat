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


            int NumberPhone = x["numbers"].i();


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
            try {
                crow::json::rvalue x = crow::json::load(req.body);
                if (!x || !x.has("name") || !x.has("UserIds"))
                {
                    return crow::response(400, "missing required fields");
                }

                bool isGroup = x["UserIds"].size() > 2;
                if (!isGroup)
                {
                    vector<int> UserIds;
                    for (const auto& userId : x["UserIds"])
                    {
                        UserIds.push_back(userId.i());
                    }
                    SQLite::Statement checkQuery(db.db, "SELECT C.Id FROM Chats"
                        "INNER JOIN UserChat AS UC1 ON C.Id = UC1.chatId"
                        "INNER JOIN UserChat AS UC2 ON C.Id = UC2.chatId"
                        "WHERE C.isGroup = 0"
                        "AND UC1.UserId = ?"
                        "AND UC2.UserId = ?"
                        "AND(SELECT COUNT(*) FROM UserChat WHERE chatId = C.Id) =2");
                    checkQuery.bind(1, UserIds[0]);
                    checkQuery.bind(2, UserIds[1]);

                    if (checkQuery.executeStep())
                    {
                        crow::json::wvalue response;
                        response["error"] = "Private chat with this user already exists";
                        response["existingChatId"] = checkQuery.getColumn(0).getInt();
                        return crow::response(409, response);
                    }
                }
                db.db.exec("BEGIN TRANSACTION");
                SQLite::Statement chatQuery(db.db, "INSERT INTO Chats(name, isGroup,createAt)VALUES(?,?,?)");
                chatQuery.bind(1, x["name"].s());
                chatQuery.bind(2, isGroup ? 1 : 0);
                chatQuery.bind(3, db.getDateTime());
                chatQuery.exec();

                int chatId = static_cast<int>(db.db.getLastInsertRowid());
                SQLite::Statement userChatQuery(db.db, "INSERT INTO UserChat(UserId,ChatId, JonedAt) VALUES(?,?,?)");
                for (const auto& userId : x["UserIds"])
                {
                    userChatQuery.reset();
                    userChatQuery.bind(1, userId.i());
                    userChatQuery.bind(2, chatId);
                    userChatQuery.bind(3, db.getDateTime());
                    userChatQuery.exec();

                }
                db.db.exec("COMMIT");
                crow::json::wvalue response;
                response["Id"] = chatId;
                response["message"] = "Chat created";
                return crow::response(201, response);

            }
            catch (const std::exception& e) {
                db.db.exec("ROLLBACK");
                return crow::response(400, e.what());
            }
        });
    CROW_ROUTE(app, "/chats/user/<int>")([&](int userId) {
        try {
            SQLite::Statement query(db.db,
                "SELECT C.name, C.isGroup, C.createAt, M.Message as LastMessage, M.SendDate AS LastMessageTime FROM Chats AS C"
                "INNER JOIN UserChat as UC ON UC.chatId = C.Id"
                "LEFT JOIN Message M ON M.Id = ( SELECT Id FROM Message WHERE chatId = C.Id ORDER BY SendDate DESC LIMIT 1)"
                "LEFT JOIN User U ON U.Id = M.UserId"
                "WHERE UC.UserId = ? "
                "ORDER BY COALESCE(M.SendDate, C.createAt) DESC");
            query.bind(1, userId);
            crow::json::wvalue::list chats;
            while (query.executeStep())
            {
                crow::json::wvalue chat;
                int chatId = query.getColumn(0).getInt();
                bool IsGroup = query.getColumn(2).getInt() == 1;
                chat["Id"] = chatId;
                chat["IsGroup"] = IsGroup;
                chat["CreatedAt"] = query.getColumn(3).getText();
                if (IsGroup)
                {
                    chat["name"] = query.getColumn(1).getText();
                }
                else {
                    SQLite::Statement otherUserQuery(db.db,
                        "SELECT U.Name FROM Users AS U"
                        "INNER JOIN UserChat AS UC ON UC.UserId = U.Id"
                        "WHERE UC.chatId = ? AND U.Id != ?");
                    otherUserQuery.bind(1, chatId);
                    otherUserQuery.bind(2, userId);
                    if (otherUserQuery.executeStep())
                    {
                        chat["name"] = otherUserQuery.getColumn(0).getText();
                    }
                    else {
                        chat["name"] = query.getColumn(1).getText();
                    }

                    if (!query.getColumn(4).isNull())
                    {
                        chat["lastMessage"] = query.getColumn(4).getText();
                        chat["lastMessageTime"] = query.getColumn(5).getText();
                        chat["lastMessageUser"] = query.getColumn(6).getText();
                    }
                    chats.push_back(move(chat));

                }
                crow::json::wvalue response;
                response["chats"] = move(chats);
                return crow::response(200, response);

            }
        }
        catch (exception& e) {
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