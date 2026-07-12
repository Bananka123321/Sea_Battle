#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <iomanip>
#include <unordered_map>

#include "lobby.h"

class LobbyManager {
public:
    LobbyManager() = default;
    
    LobbyManager(const LobbyManager&) = delete;
    LobbyManager& operator=(const LobbyManager&) = delete;

    std::shared_ptr<Lobby> CreateLobby(const std::string& code, std::shared_ptr<ClientSession> host);
    std::shared_ptr<Lobby> GetLobby(const std::string& code);
    std::shared_ptr<Lobby> GetPlayerLobby(const std::shared_ptr<ClientSession>& player);
    
    void RemoveLobby(const std::string& code);
    void RemovePlayerFromLobby(const std::shared_ptr<ClientSession>& player);
    
    std::string GenerateLobbyCode();

private:
    std::unordered_map<std::string, std::shared_ptr<Lobby>> lobbies_;
    mutable std::mutex mutex_;
};