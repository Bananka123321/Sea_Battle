#pragma once
#include <string>
#include <memory>

class ClientSession;

class Lobby {
public:
    Lobby(const std::string& code, std::shared_ptr<ClientSession> creator);

    const std::string& getCode() const;

    bool addPlayer(std::shared_ptr<ClientSession> player);
    void removePlayer(std::shared_ptr<ClientSession> player);

    void setReady(std::shared_ptr<ClientSession> player);
    bool isBothReady() const;

    std::shared_ptr<ClientSession> getPlayer1() const;
    std::shared_ptr<ClientSession> getPlayer2() const;

    bool isFull() const;
    bool containsPlayer(std::shared_ptr<ClientSession> player) const;

private:
    std::string code;
    std::shared_ptr<ClientSession> player1;  //Создатель
    std::shared_ptr<ClientSession> player2;  //Второй игрок
    bool player1Ready;
    bool player2Ready;
};
