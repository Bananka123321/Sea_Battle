#include "../include/session_manager.h"

void SessionManager::Add(std::shared_ptr<ClientSession> client){
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.push_back(client);
}

void SessionManager::Remove(const std::shared_ptr<ClientSession>& client) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = std::remove(sessions_.begin(), sessions_.end(), client);
    sessions_.erase(it, sessions_.end());
}


const std::vector<std::shared_ptr<ClientSession>>& SessionManager::GetAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    return sessions_;
}

void SessionManager::UpdateActivity(const std::shared_ptr<ClientSession>& client) {
    using Clock = std::chrono::system_clock;
    using Duration = std::chrono::milliseconds;
    int64_t now = std::chrono::duration_cast<Duration>(Clock::now().time_since_epoch()).count();
    client->SetLastActivity(now);
}