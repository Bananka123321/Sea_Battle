#pragma once
#include <string>
#include <QObject>
#include <iostream>
#include <unordered_map>

#include "json.hpp"
#include "Message.h"
#include "protocol.h"

class Handler : public QObject{
    Q_OBJECT
public:
    Handler();

    void handleMessage(const std::string& msg);

private:

    std::unordered_map<std::string, std::function<void(const nlohmann::json&)>> handlers;

    void onMessage(const int from, const int to, const std::string& text);
    void onError(const std::string& text);
    void onConnectionResponse(const bool& success);

    void onLobbyCreated(const std::string& code);
    void onLobbyJoined(bool success, const std::string& username);
    void onPlayerJoined(const std::string& username);
    void onPlayerLeft();
    void onPlayerReadyResponse(bool success);
    void onGameStarted(bool yourTurn);
    void onShotResult(int row, int column, int result, bool yourTurn);
    void onGameOver(const std::string& winner);

signals:
    void S_Message(const int sender, const int receiver, const std::string& text);

    void S_ConnectionSucess();

    void S_LobbyCreated(const std::string& code);
    void S_LobbyJoined(bool success, const std::string& username);
    void S_PlayerJoined(const std::string& username);
    void S_PlayerLeft();
    void S_EnemyReady(bool success);
    void S_GameStarted(bool yourTurn);
    void S_ShotResult(int row, int column, int result, bool yourTurn);
    void S_onGameOver(const std::string& winner);

};
