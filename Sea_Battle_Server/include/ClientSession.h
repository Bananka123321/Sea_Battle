#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <memory>

#include "../common/PacketIO.h"
#include "lobby.h"

class ClientSession {
public:
    ClientSession(int sock);
    ~ClientSession();

    const int getSocket() const;
    const std::string& getUsername() const;

    const int& getUserId() const;
    void setUser(const int& new_id, const std::string& new_username);

    bool getIsAuthentificated() const;
    void setIsAuthentificated(bool value);

    bool send(const std::string& message);
    bool receive(std::string& message);

    int64_t getLastActivity() const;
    void setLastActivity(const int64_t& newTimestamp);

    bool getConnected() const;
    void setConnected(const bool& newState);

    void setCurrentLobby(std::shared_ptr<Lobby> lobby);
    std::shared_ptr<Lobby> getCurrentLobby() const;

private:
    int socket;
    int user_id;
    std::string username;

    bool isAuthentificated;
    std::atomic<int64_t> last_activity_time{0};
    std::atomic<bool> connected{false};

    std::shared_ptr<Lobby> currentLobby;
};