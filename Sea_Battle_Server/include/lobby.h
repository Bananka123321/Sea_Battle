#pragma once

#include <string>
#include <memory>

class ClientSession;

class Lobby {
public:
    Lobby(const std::string& code, std::shared_ptr<ClientSession> creator);

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;

    const std::string& GetCode() const;

    bool AddPlayer(const std::shared_ptr<ClientSession>& player);
    void RemovePlayer(const std::shared_ptr<ClientSession>& player);

    bool SetReady(const std::shared_ptr<ClientSession>& player);
    bool IsBothReady() const;

    std::shared_ptr<ClientSession> GetPlayer1() const;
    std::shared_ptr<ClientSession> GetPlayer2() const;

    bool IsFull() const;
    bool ContainsPlayer(const std::shared_ptr<ClientSession>& player) const;

    bool GetPlayer1Ready() const;
    bool GetPlayer2Ready() const;

private:
    std::string code_;
    std::shared_ptr<ClientSession> player1_;  //Создатель
    std::shared_ptr<ClientSession> player2_;  //Второй игрок
    bool player1_ready_;
    bool player2_ready_;
};
