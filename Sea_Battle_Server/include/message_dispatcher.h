#pragma once

#include <functional>

#include "../include/session_manager.h"

class MessageDispatcher {
public:
    using DisconnectCallback = std::function<void(std::shared_ptr<ClientSession>)>;

    explicit MessageDispatcher(SessionManager* sm);
    
    MessageDispatcher(const MessageDispatcher&) = delete;
    MessageDispatcher& operator=(const MessageDispatcher&) = delete;

    void SetDisconnectCallback(DisconnectCallback callback);

    void SendTo(const std::shared_ptr<ClientSession>& client, const std::string& msg);

private:
    SessionManager* session_manager_;
    DisconnectCallback on_disconnect_;
};