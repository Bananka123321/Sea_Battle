#include "../include/ClientSession.h"

ClientSession::ClientSession(int sock) : socket(sock), isAuthentificated(false), last_activity_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) {}

ClientSession::~ClientSession() {}

const int ClientSession::getSocket() const {
    return socket;
}

const std::string& ClientSession::getUsername() const {
    return username;
}

const int& ClientSession::getUserId() const {
    return user_id;
}

bool ClientSession::send(const std::string& message) {
    return PacketIO::sendPacket(socket, message);
}   

bool ClientSession::receive(std::string& message) {
    return PacketIO::recvPacket(socket, message);
}

void ClientSession::setUser(const int& new_id, const std::string& new_username) {
    username = new_username;
    user_id = new_id;
}

bool ClientSession::getIsAuthentificated() const {
    return isAuthentificated;
}

void ClientSession::setIsAuthentificated(bool value) {
    isAuthentificated = value;
}

int64_t ClientSession::getLastActivity() const {
    return last_activity_time.load(std::memory_order_relaxed);
}

void ClientSession::setLastActivity(const int64_t& newTimestamp) {
    last_activity_time.store(newTimestamp);
}

bool ClientSession::getConnected() const {
    return connected.load(std::memory_order_relaxed);
}

void ClientSession::setConnected(const bool& newState) {
    connected.store(newState);
}