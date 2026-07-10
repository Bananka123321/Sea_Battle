#pragma once
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

#include "../common/protocol.h"
#include "../common/Validator.h"
#include "SessionManager.h"
#include "MessageDispatcher.h"
#include "LobbyManager.h"

class Handler {
public:
    Handler(SessionManager& sm);
    ~Handler();

    void handleMessage(std::shared_ptr<ClientSession> client, std::string& msg);
    void setDisconnectHandler(std::function<void(std::shared_ptr<ClientSession>)> cb);

    void notifyPlayerLeft(std::shared_ptr<ClientSession> client, const std::string& playerName);
    void removeEmptyLobby(const std::string& code);
    
private:
    SessionManager& sessionManager;
    MessageDispatcher dispatcher;

    std::unordered_map<std::string, std::function<void(std::shared_ptr<ClientSession>, const nlohmann::json&)>> handlers;

    LobbyManager lobbyManager;

private:
    void loginRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void registerRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void privateMessage(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void searchUserRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void ping(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void resumeConnectionRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    
    void authSuccess(std::shared_ptr<ClientSession> client, const int& id, const std::string& username);

    std::string generateToken();

    void createLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void leaveLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void joinLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
    void playerReadyRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j);
};