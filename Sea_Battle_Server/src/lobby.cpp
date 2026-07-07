#include "lobby.h"
#include "ClientSession.h"

Lobby::Lobby(const std::string& code, std::shared_ptr<ClientSession> creator)
    : code(code), state(LobbyState::WAITING_FOR_PLAYERS),
    player1(creator), player2(nullptr), player1Ready(false), player2Ready(false) {
}

const std::string& Lobby::getCode() const {
    return code;
}

LobbyState Lobby::getState() const {
    return state;
}

bool Lobby::addPlayer(std::shared_ptr<ClientSession> player) {
    if (player2 != nullptr) return false;

    player2 = player;
    return true;
}

void Lobby::removePlayer(std::shared_ptr<ClientSession> player) {
    if (player1 == player) {
        player1 = nullptr;
        player1Ready = false;
    } else if (player2 == player) {
        player2 = nullptr;
        player2Ready = false;
    }

    if (player1 != nullptr && player2 == nullptr)
        state = LobbyState::WAITING_FOR_PLAYERS;
}

void Lobby::setReady(std::shared_ptr<ClientSession> player) {
    if (player1 == player)
        player1Ready = true;
    else if (player2 == player)
        player2Ready = true;

    if (player1Ready && player2Ready)
        state = LobbyState::BOTH_READY;
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
