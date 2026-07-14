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
#include "../include/client_session.h"
#include "../include/session_manager.h"
#include "../include/handler.h"


class TCPServer {
public:
    explicit TCPServer(int port_);
    ~TCPServer();

    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    bool Start();
    void Stop();

private:
    static constexpr int64_t HEARTBEAT_INTERVAL_MS = 30000; //Период проверки соединения
    static constexpr int64_t SESSION_TIMEOUT_MS = 90000; //Период за который клиент ни разу не отправил пинг

    int port_;
    int server_socket_;

    std::atomic<bool> serverRunning_{false};
    std::atomic<bool> monitorRunning_{false};

    std::thread monitor_thread_;

    SessionManager* session_manager_;
    Handler handler_;

    friend class MessageDispatcher;

private:
    bool SetupSocket();
    void Run();
    void ClientDisconnect(const std::shared_ptr<ClientSession>& client);
    void HandleClient(std::shared_ptr<ClientSession> client);

    void StartClientMonitoring();
};