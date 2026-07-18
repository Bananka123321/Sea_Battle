#include "../include/client_session.h"
#include "lobby.h"

ClientSession::ClientSession(int sock) : socket_(sock), isAuthentificated_(false), last_activity_time_(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) {}

ClientSession::~ClientSession() {}

int ClientSession::GetSocket() const {
    return socket_;
}

const std::string& ClientSession::GetUsername() const {
    return username_;
}

int ClientSession::GetUserId() const {
    return user_id_;
}

bool ClientSession::Send(const std::string& message) {
    return PacketIO::sendPacket(socket_, message);
}   

bool ClientSession::Receive(std::string& message) {
    return PacketIO::recvPacket(socket_, message);
}

void ClientSession::SetUser(int new_id, const std::string& new_username) {
    username_ = new_username;
    user_id_ = new_id;
}

void ClientSession::SetUsername(const std::string& newUsername) {
    username_ = newUsername;
}

bool ClientSession::GetIsAuthentificated() const {
    return isAuthentificated_;
}

void ClientSession::SetIsAuthentificated(bool value) {
    isAuthentificated_ = value;
}

int64_t ClientSession::GetLastActivity() const {
    return last_activity_time_.load(std::memory_order_relaxed);
}

void ClientSession::SetLastActivity(const int64_t& newTimestamp) {
    last_activity_time_.store(newTimestamp);
}

bool ClientSession::GetConnected() const {
    return connected_.load(std::memory_order_relaxed);
}

void ClientSession::SetConnected(bool newState) {
    connected_.store(newState);
}

void ClientSession::SetCurrentLobby(std::shared_ptr<Lobby> lobby) {
    current_lobby_ = lobby;
}

std::shared_ptr<Lobby> ClientSession::GetCurrentLobby() const {
    return current_lobby_;
}