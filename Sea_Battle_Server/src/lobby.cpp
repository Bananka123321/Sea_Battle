#include "lobby.h"
#include "ClientSession.h"

Lobby::Lobby(const std::string& code, std::shared_ptr<ClientSession> creator)
    : code(code), player1(creator), player2(nullptr), player1Ready(false), player2Ready(false) {
}

const std::string& Lobby::getCode() const {
    return code;
}

bool Lobby::addPlayer(std::shared_ptr<ClientSession> player) {
    if (player1 == nullptr) {
        player1 = player;
        return true;
    }
    if (player2 == nullptr) {
        player2 = player;
        return true;
    }
    return false;
}

void Lobby::removePlayer(std::shared_ptr<ClientSession> player) {
    if (player1 == player) {
        player1 = nullptr;
        player1Ready = false;
    } else if (player2 == player) {
        player2 = nullptr;
        player2Ready = false;
    }
}

bool Lobby::setReady(std::shared_ptr<ClientSession> player) {
    if (player1 == player) {
        player1Ready = !player1Ready;
        return player1Ready;
    }
    else if (player2 == player) {
        player2Ready = !player2Ready;
        return player2Ready;
    }
    return false; //Игрок не найден
}

bool Lobby::isBothReady() const {
    return player1Ready && player2Ready;
}

std::shared_ptr<ClientSession> Lobby::getPlayer1() const {
    return player1;
}

std::shared_ptr<ClientSession> Lobby::getPlayer2() const {
    return player2;
}

bool Lobby::isFull() const {
    return player2 != nullptr;
}

bool Lobby::containsPlayer(std::shared_ptr<ClientSession> player) const {
    return player1 == player || player2 == player;
}

bool Lobby::getPlayer1Ready() const {
    return player1Ready;
}

bool Lobby::getPlayer2Ready() const {
    return player2Ready;
}