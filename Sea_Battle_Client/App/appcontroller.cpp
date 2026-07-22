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

    connect(mainW, &MainWindow::leaveLobbyRequest, this, [this](){
        router_->leaveLobbyRequest();
    });

    connect(mainW, &MainWindow::playerReady, this, [this](const std::vector<ShipData>& ships){
        router_->playerReady(ships);
    });

    connect(mainW, &MainWindow::shootRequest, this, [this](int row, int column) {
        router_->shootRequest(row, column);
    });

    connect(mainW, &MainWindow::revengeRequest, this, [this]() {
        router_->revengeRequest();
    });

    connect(handler_, &Handler::S_Message, mainW, &MainWindow::onReceiveChatMessage, Qt::QueuedConnection);

    connect(handler_, &Handler::S_onGameOver, mainW, &MainWindow::gameOver, Qt::QueuedConnection);
}

AppController::~AppController() {}
