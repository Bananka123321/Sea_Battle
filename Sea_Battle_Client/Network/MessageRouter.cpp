#include "MessageRouter.h"

MessageRouter::MessageRouter() {}
\
void MessageRouter::setSocket(int socket) {
    std::lock_guard<std::mutex> lock(mutex);
    socket_ = socket;
}

void MessageRouter::sendMessage(const std::string& text) {
    std::string request = protocol::chatMessage(text);
    sendPacket(request);
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

void MessageRouter::revengeRequest() {
    std::string request = protocol::revenge();
    sendPacket(request);
}

void MessageRouter::leaveLobbyRequest() {
    std::string request = protocol::leaveLobby();
    sendPacket(request);
}

void MessageRouter::sendPacket(const std::string& msg, bool force) {
    if(!socket_)
        return;

    PacketIO::sendPacket(socket_, msg);
}
