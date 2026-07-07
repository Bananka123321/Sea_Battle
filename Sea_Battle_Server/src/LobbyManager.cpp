#include "LobbyManager.h"
#include <random>
#include <sstream>
#include <iomanip>

std::shared_ptr<Lobby> LobbyManager::createLobby(const std::string& code, std::shared_ptr<ClientSession> host) {
    std::lock_guard<std::mutex> lock(mutex);
    
    auto lobby = std::make_shared<Lobby>(code, host);
    lobbies[code] = lobby;
    return lobby;
}

std::shared_ptr<Lobby> LobbyManager::getLobby(const std::string& code) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = lobbies.find(code);
    if (it != lobbies.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<Lobby> LobbyManager::getPlayerLobby(std::shared_ptr<ClientSession> player) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& [code, lobby] : lobbies) {
        if (lobby->containsPlayer(player))
            return lobby;
    }
    return nullptr;
}

void LobbyManager::removeLobby(const std::string& code) {
    std::lock_guard<std::mutex> lock(mutex);
    lobbies.erase(code);
}

void LobbyManager::removePlayerFromLobby(std::shared_ptr<ClientSession> player) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& [code, lobby] : lobbies) {
        if (lobby->containsPlayer(player)) {
            lobby->removePlayer(player);
            
            if (lobby->getPlayer1() == nullptr && lobby->getPlayer2() == nullptr)
                lobbies.erase(code);
            break;
        }
    }
}

std::string LobbyManager::generateLobbyCode() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 35);
    
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::stringstream ss;
    //Надо доделать, пока хз как правильнее будет
    return ss.str();
}