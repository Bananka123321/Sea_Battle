#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "game_session.h"

class GameManager {
public:
    GameManager() = default;

    void createGame(const std::string& lobbyCode, std::shared_ptr<ClientSession> p1, std::shared_ptr<ClientSession> p2);
    
    std::shared_ptr<GameSession> getGame(const std::string& lobbyCode);
    
    void removeGame(const std::string& lobbyCode);

private:
    std::unordered_map<std::string, std::shared_ptr<GameSession>> games_;
    mutable std::mutex mutex_;
};