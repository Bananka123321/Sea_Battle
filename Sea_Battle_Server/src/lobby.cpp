#include "lobby.h"
#include "client_session.h"
#include "game_session.h"

Lobby::Lobby(const std::string& code, std::shared_ptr<ClientSession> creator)
    : code_(code), player1_(creator), player2_(nullptr), player1_ready_(false), player2_ready_(false) {
}

const std::string& Lobby::GetCode() const {
    return code_;
}

bool Lobby::AddPlayer(const std::shared_ptr<ClientSession>& player) {
    if (player1_ == nullptr) {
        player1_ = player;
        return true;
    }
    if (player2_ == nullptr) {
        player2_ = player;
        return true;
    }
    return false;
}

void Lobby::RemovePlayer(const std::shared_ptr<ClientSession>& player) {
    if (player1_ == player) {
        player1_ = nullptr;
        player1_ready_ = false;
    } else if (player2_ == player) {
        player2_ = nullptr;
        player2_ready_ = false;
    }
}

bool Lobby::SetReady(const std::shared_ptr<ClientSession>& player) {
    if (player1_ == player) {
        player1_ready_ = !player1_ready_;
        return player1_ready_;
    }
    
    else if (player2_ == player) {
        player2_ready_ = !player2_ready_;
        return player2_ready_;
    }

    return false; //Игрок не найден
}

bool Lobby::IsBothReady() const {
    return player1_ready_ && player2_ready_;
}

std::shared_ptr<ClientSession> Lobby::GetPlayer1() const {
    return player1_;
}

std::shared_ptr<ClientSession> Lobby::GetPlayer2() const {
    return player2_;
}

bool Lobby::IsFull() const {
    return player2_ != nullptr;
}

bool Lobby::ContainsPlayer(const std::shared_ptr<ClientSession>& player) const {
    return player1_ == player || player2_ == player;
}

bool Lobby::GetPlayer1Ready() const {
    return player1_ready_;
}

bool Lobby::GetPlayer2Ready() const {
    return player2_ready_;
}

GameSession* Lobby::getGame() const {
    return game_.get();
}

void Lobby::createGame() {
    game_ = std::make_unique<GameSession>(player1_, player2_);
}

void Lobby::destroyGame() {
    game_.reset();
}

void Lobby::resetGame() {
    game_.reset();

    player1_ready_ = false;
    player2_ready_ = false;
}