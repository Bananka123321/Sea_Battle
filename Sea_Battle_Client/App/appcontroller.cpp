#define NOMINMAX//Для функции max
#include "appcontroller.h"

AppController::AppController(MessageRouter* router, Handler* handler, TCPClient* client) : router_(router), handler_(handler), client_(client) {}

void AppController::AttachUI(MainWindow* mainW) {
    connect(mainW, &MainWindow::sendMessageRequest, this, [this](const std::string& text) {
        router_->sendMessage(text);
    });

    connect(mainW, &MainWindow::createLobbyRequest, this, [this](const std::string& username){
        router_->createLobbyRequest(username);
    });

    connect(mainW, &MainWindow::joinLobbyRequest, this, [this](const std::string& username, const std::string& code){
        router_->joinLobbyRequest(username, code);
    });

    connect(mainW, &MainWindow::playerReady, this, [this](const std::vector<ShipData>& ships){
        router_->playerReady(ships);
    });

    connect(mainW, &MainWindow::shootRequest, this, [this](int row, int column) {
        router_->shootRequest(row, column);
    });

    connect(this, &AppController::ping, router_, &MessageRouter::ping, Qt::QueuedConnection);

    connect(client_, &TCPClient::connectionLose, this, [this](){
        stopPing();
        startReconnect();
    });

    connect(client_, &TCPClient::connected, this, [this](){
        // router_->resumeConnectionRequest(state_->getCurrentToken());
    });

    connect(handler_, &Handler::S_ConnectionSucess, this, [this](){
        router_->setReconnecting(false);
        startPing();
    });


    connect(handler_, &Handler::S_Message, mainW, &MainWindow::onReceiveChatMessage, Qt::QueuedConnection);
}

AppController::~AppController() {
    stopPing();
}

void AppController::startPing() {
    if(pingTimer) return;

    pingTimer = new QTimer(this);
    connect(pingTimer, &QTimer::timeout, this, [this](){
        emit ping();
    });
    pingTimer->start(PING_TIME_MS);
}

void AppController::stopPing() {
    if(!pingTimer) return;
    pingTimer->stop();
    pingTimer->deleteLater();
    pingTimer = nullptr;
}

///Функция экспоненциальной проверки переподключения к серверу
///После 10 попыток перестаём подключаться(можно убрать это)
void AppController::startReconnect() {
    router_->setReconnecting(true);

    if(reconnectTimer && reconnectTimer->isActive()) return;
    if(!reconnectTimer) {
        reconnectTimer = new QTimer(this);
        reconnectTimer->setSingleShot(true);

        connect(reconnectTimer, &QTimer::timeout, this, [this](){
            bool ok = client_->start();
            if(ok) {
                reconnectAttempts = 0;
            } else {
                reconnectAttempts++;
                if(reconnectAttempts <= 10) {
                    int delay = std::min(1000 * (1 << reconnectAttempts), MAX_RECONNECT_TIME_MS);
                    reconnectTimer->start(delay);
                } else {
                    router_->setReconnecting(false);
                }
            }
        });
    }

    reconnectTimer->start(1000);
}
