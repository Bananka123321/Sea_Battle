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

    void loginRequest(const std::string& login, const std::string& password);
    void registerRequest(const std::string& login, const std::string& password);
    void sendMessage(const int& from, const int& to, const std::string& text);
    void searchUser(const std::string& text);
    void historyRequest(int peer_id, int last_msg_id);
    void getDialogsRequest();
    void ping();
    void resumeConnectionRequest(const std::string& token);

    void setReconnecting(bool value);

private:
    int socket_;
    std::mutex mutex;
    std::atomic<bool> isReconnecting{false};

private:
    void sendPacket(const std::string& msg, bool force = false);
};
