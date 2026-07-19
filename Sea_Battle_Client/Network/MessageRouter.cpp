#include "MessageRouter.h"

MessageRouter::MessageRouter() {}
\
void MessageRouter::setSocket(int socket) {
    std::lock_guard<std::mutex> lock(mutex);
    socket_ = socket;
}

void MessageRouter::setReconnecting(bool value) {
    isReconnecting.store(value);
}

void MessageRouter::sendMessage(const int& from, const int& to, const std::string& text) {
    std::string request = protocol::privateMessage(from, to, text);
    sendPacket(request);
}

void MessageRouter::ping() {
    std::string request = protocol::ping();
    sendPacket(request);
}

void MessageRouter::resumeConnectionRequest(const std::string& token) {
    std::string request = protocol::resumeConnectionRequest(token);
    sendPacket(request, true);
}

void MessageRouter::createLobbyRequest(const std::string& username) {
    std::string request = protocol::createLobby(username);
    sendPacket(request);
}

void MessageRouter::joinLobbyRequest(const std::string& username, const std::string& code) {
    std::string request = protocol::joinLobby(username, code);
    sendPacket(request);
}

void MessageRouter::playerReady(const std::vector<ShipData>& ships) {
    std::string request = protocol::playerReadyRequest(ships);
    sendPacket(request);
}

void MessageRouter::shootRequest(int row, int column) {
    std::string request = protocol::shoot(row, column);
    sendPacket(request);
}

void MessageRouter::sendPacket(const std::string& msg, bool force) {
    if(!force && isReconnecting.load())
        return;

    std::lock_guard<std::mutex> lock(mutex);
    if(!force && isReconnecting.load())
        return;

    if(!socket_)
        return;

    PacketIO::sendPacket(socket_, msg);
}
