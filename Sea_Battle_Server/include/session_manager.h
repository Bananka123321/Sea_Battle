#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <algorithm>

#include "client_session.h"

class SessionManager {
public:
    void Add(std::shared_ptr<ClientSession> client);
    void Remove(const std::shared_ptr<ClientSession>& client);

    const std::vector<std::shared_ptr<ClientSession>>& GetAll();

    void UpdateActivity(const std::shared_ptr<ClientSession>& client);
    std::optional<int> checkToken(const std::string& token);

private:
    std::vector<std::shared_ptr<ClientSession>> sessions_;
    std::mutex mutex_;
};