#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <memory>

#include "../common/PacketIO.h"
#include "lobby.h"

class ClientSession {
public:
    explicit ClientSession(int sock);
    ~ClientSession();
    
    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;

    int GetSocket() const;
    const std::string& GetUsername() const;

    int GetUserId() const;
    void SetUser(int new_id, const std::string& new_username);
    void SetUsername(const std::string& newUsername);

    bool GetIsAuthentificated() const;
    void SetIsAuthentificated(bool value);

    bool Send(const std::string& message);
    bool Receive(std::string& message);

    int64_t GetLastActivity() const;
    void SetLastActivity(const int64_t& newTimestamp);

    bool GetConnected() const;
    void SetConnected(bool newState);

    void SetCurrentLobby(std::shared_ptr<Lobby> lobby);
    std::shared_ptr<Lobby> GetCurrentLobby() const;

private:
    int socket_;
    int user_id_;
    std::string username_;

    bool isAuthentificated_;
    std::atomic<int64_t> last_activity_time_{0};
    std::atomic<bool> connected_{false};

    std::shared_ptr<Lobby> current_lobby_;
};