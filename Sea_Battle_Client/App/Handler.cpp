#include "Handler.h"

Handler::Handler() {
    handlers["loginResponse"] = [this] (const nlohmann::json& j) {
        onLoginResponse(j["success"], j["user_id"], j["username"], j["token"], j["error"]);
    };

    handlers["registerResponse"] = [this] (const nlohmann::json& j) {
        onRegisterResponse(j["success"], j["user_id"], j["username"], j["token"], j["error"]);
    };

    handlers["userList"] = [this] (const nlohmann::json& j) {
        onUserList(j["users"]);
    };

    handlers["privateMessage"] = [this] (const nlohmann::json& j) {
        onMessage(j["from"], j["to"], j["text"]);
    };

    handlers["searchUserResponse"] = [this] (const nlohmann::json& j) {
        onUserSearch(j["result"]);
    };

    handlers["error"] = [this] (const nlohmann::json& j) {
        onError(j["message"]);
    };

    handlers["resumeConnectionResponse"] = [this] (const nlohmann::json& j) {
        onConnectionResponse(j["success"]);
    };
}


///Функция парсит пришедший пакет и вызывает соответствующую лямбда функцию из
///заранее готового списка handlers заполненного в конструкторе выше
void Handler::handleMessage(const std::string& msg) {
    try {
        auto j = nlohmann::json::parse(msg);
        std::string type = j["type"];//Получает тип пакета

        if (handlers.find(type) != handlers.end())
            handlers[type](j);//если такой имеется вызывает соответствующую функцию
        else
            std::cerr << "Unknown message type " << type << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse ERROR " << e.what() << std::endl;
    }
}

//=============================================================================================

void Handler::onLoginResponse(const bool& success, const int user_id, const std::string& login, const std::string& token, const std::string& reason) {
    if (success) {
        emit S_loginSuccess(login, user_id, token);
    } else
        emit S_loginFailed(reason);
}

void Handler::onRegisterResponse(const bool& success, const int user_id, const std::string& login, const std::string& token, const std::string& reason) {
    if (success) {
        emit S_registerSuccess(login, user_id, token);
    } else
        emit S_registerFailed(reason);
}

void Handler::onUserList(const std::unordered_map<int, std::string>& users) {
    emit S_userList(users);
}

void Handler::onMessage(const int from, const int to, const std::string& text) {
    emit S_Message(from, to, text);
}

void Handler::onUserSearch(const std::vector<User>& users) {
    emit S_UserSearch(users);
}

void Handler::onError(const std::string& text) {
    std::cerr << text << '\n';
}

void Handler::onConnectionResponse(const bool& success) {
    if(success)
        emit S_ConnectionSucess();
}
