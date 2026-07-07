#pragma once
#include <map>
#include <memory>
#include <mutex>

#include "lobby.h"

class LobbyManager {
public:
    LobbyManager() = default;
    
    std::shared_ptr<Lobby> createLobby(const std::string& code, std::shared_ptr<ClientSession> host);
    std::shared_ptr<Lobby> getLobby(const std::string& code);
    std::shared_ptr<Lobby> getPlayerLobby(std::shared_ptr<ClientSession> player);
    
    void removeLobby(const std::string& code);
    void removePlayerFromLobby(std::shared_ptr<ClientSession> player);
    
    std::string generateLobbyCode();

private:
    std::map<std::string, std::shared_ptr<Lobby>> lobbies;
    mutable std::mutex mutex;
};