///Файл предназначен для отправки пакетов с клиента

#pragma once

#include <string>
#include <QObject>
#include <mutex>
#include <atomic>

#include "protocol.h"
#include "PacketIO.h"

class MessageRouter : public QObject{
    Q_OBJECT
public:
    MessageRouter();

    void setSocket(int socket);

    void sendMessage(const std::string& text);

    void createLobbyRequest(const std::string& username);
    void joinLobbyRequest(const std::string& username, const std::string& code);
    void leaveLobbyRequest();
    void playerReady(const std::vector<ShipData>& ships);
    void shootRequest(int row, int column);
    void revengeRequest();


private:
    int socket_;
    std::mutex mutex;

private:
    void sendPacket(const std::string& msg, bool force = false);
};
