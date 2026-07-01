#pragma once

#include <thread>
#include <unistd.h>
#include <mutex>
#include <atomic>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "../common/protocol.h"
#include "../include/ClientSession.h"
#include "../include/SessionManager.h"
#include "../include/Handler.h"

static int64_t HEARTBEAT_INTERVAL_MS = 30000; //Период проверки соединения
static int64_t SESSION_TIMEOUT_MS = 90000; //Период за который клиент ни разу не отправил пинг

class TCPServer {
public:
    TCPServer(int port, Handler handler);
    ~TCPServer();

    bool start();
    void stop();

private:
    int port;
    int serverSocket;

    std::atomic<bool> serverRunning{false};
    std::atomic<bool> monitorRunning{false};

    std::thread monitor_thread;

    SessionManager sessionManager_;
    Handler handler_;

    friend class MessageDispatcher;

    bool setupSocket();
    void run();
    void clientDisconnect(std::shared_ptr<ClientSession> client);
    void handleClient(std::shared_ptr<ClientSession> client);

    void startClientMonitoring();
};