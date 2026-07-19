#include "lobby_manager.h"
#include "client_session.h"
#include "game_session.h"

std::shared_ptr<Lobby> LobbyManager::CreateLobby(const std::string& code, std::shared_ptr<ClientSession> host) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto lobby = std::make_shared<Lobby>(code, host);
    lobbies_[code] = lobby;
    return lobby;
}

std::shared_ptr<Lobby> LobbyManager::GetLobby(const std::string& code) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = lobbies_.find(code);
    if (it != lobbies_.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<Lobby> LobbyManager::GetPlayerLobby(const std::shared_ptr<ClientSession>& player) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [code_, lobby] : lobbies_) {
        if (lobby->ContainsPlayer(player))
            return lobby;
    }
    return nullptr;
}

void LobbyManager::RemoveLobby(const std::string& code) {
    std::lock_guard<std::mutex> lock(mutex_);
    lobbies_.erase(code);
}

void LobbyManager::RemovePlayerFromLobby(const std::shared_ptr<ClientSession>& player) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [code_, lobby] : lobbies_) {
        if (lobby->ContainsPlayer(player)) {
            lobby->RemovePlayer(player);
            
            if (lobby->GetPlayer1() == nullptr && lobby->GetPlayer2() == nullptr)
                lobbies_.erase(code_);
            break;
        }
    }
}

std::string LobbyManager::GenerateLobbyCode() {
    static const std::string chars = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, chars.size() - 1);
    
    std::string code_;
    code_.reserve(6);
    for (int i = 0; i < 6; i++)
        code_ += chars[dis(gen)];

    return code_;
}