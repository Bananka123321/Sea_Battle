#include "../include/message_dispatcher.h"

MessageDispatcher::MessageDispatcher(SessionManager* sm) : session_manager_(sm) {}

void MessageDispatcher::SetDisconnectCallback(DisconnectCallback callback) {
  on_disconnect_ = std::move(callback);
}

void MessageDispatcher::SendTo(const std::shared_ptr<ClientSession>& client, const std::string& msg) {
    if (!client->Send(msg) && on_disconnect_)
        on_disconnect_(client);
}