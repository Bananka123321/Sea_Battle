#pragma once
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

#include "../common/protocol.h"
#include "../common/Validator.h"
#include "session_manager.h"
#include "message_dispatcher.h"
#include "lobby_manager.h"

class Handler {
public:
    using DisconnectCallback = std::function<void(const std::shared_ptr<ClientSession>&)>;

    explicit Handler(SessionManager* session_manager);
    ~Handler();

    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;

    void HandleMessage(const std::shared_ptr<ClientSession>& client, std::string& message);
    void SetDisconnectHandler(DisconnectCallback callback);

    void NotifyPlayerLeft(const std::shared_ptr<ClientSession>& client, const std::string& player_name);
    void RemoveEmptyLobby(const std::string& code);
    
private:
    using MessageHandler = std::function<void(const std::shared_ptr<ClientSession>&, const nlohmann::json&)>;

    SessionManager* session_manager_;
    MessageDispatcher dispatcher_;
    LobbyManager lobbyManager_;

    std::unordered_map<std::string, MessageHandler> handlers_;


private:
    void LoginRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void RegisterRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void PrivateMessage(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void Ping(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void ResumeConnectionRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    
    void authSuccess(const std::shared_ptr<ClientSession>& client, int id, const std::string& username_);

    std::string generateToken();

    void CreateLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void leaveLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void joinLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
    void playerReadyRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j);
};