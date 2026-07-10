#pragma once
#include <string>
#include <QObject>
#include <iostream>
#include <unordered_map>

#include "json.hpp"
#include "Message.h"
#include "protocol.h"

class Handler : public QObject{
    Q_OBJECT
public:
    Handler();

    void handleMessage(const std::string& msg);

private:

    std::unordered_map<std::string, std::function<void(const nlohmann::json&)>> handlers;

    void onLoginResponse(const bool& success, const int user_id, const std::string& login, const std::string& token, const std::string& reason);
    void onRegisterResponse(const bool& success, const int user_id, const std::string& login, const std::string& token, const std::string& reason);
    void onUserList(const std::unordered_map<int, std::string>& users);
    void onMessage(const int from, const int to, const std::string& text);
    void onUserSearch(const std::vector<User>& users);
    void onError(const std::string& text);
    void onConnectionResponse(const bool& success);

    void onLobbyCreated(const std::string& code);
    void onLobbyJoined(bool success);
    void onPlayerJoined(const std::string& username);
    void onPlayerLeft();
    void onPlayerReadyResponse(bool success);

signals:
    void S_loginSuccess(const std::string& login, const int& user_id, const std::string& token);
    void S_loginFailed(const std::string& reason);

    void S_registerSuccess(const std::string& login, const int& user_id, const std::string& token);
    void S_registerFailed(const std::string& reason);

    void S_Message(const int sender, const int receiver, const std::string& text);
    void S_userList(const std::unordered_map<int, std::string>& users);

    void S_UserSearch(const std::vector<User>& users);

    void S_ConnectionSucess();

    void S_LobbyCreated(const std::string& code);
    void S_LobbyJoined(bool success);
    void S_PlayerJoined(const std::string& username);
    void S_PlayerLeft();
    void S_EnemyReady(bool success);

};
