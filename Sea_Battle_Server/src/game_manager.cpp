#include "game_manager.h"

void GameManager::createGame(const std::string& lobbyCode, std::shared_ptr<ClientSession> p1, std::shared_ptr<ClientSession> p2) {
    std::lock_guard<std::mutex> lock(mutex_);
    games_[lobbyCode] = std::make_shared<GameSession>(p1, p2);
}

std::shared_ptr<GameSession> GameManager::getGame(const std::string& lobbyCode) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = games_.find(lobbyCode);
    if (it != games_.end())
        return it->second;
    return nullptr;
}

void GameManager::removeGame(const std::string& lobbyCode) {
    std::lock_guard<std::mutex> lock(mutex_);
    games_.erase(lobbyCode);
}