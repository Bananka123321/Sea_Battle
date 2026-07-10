#include "LobbyManager.h"

LobbyManager::LobbyManager(Handler* handler, MainWindow* window) : handler_(handler), window_(window) {
    connect(handler_, &Handler::S_LobbyCreated, this, &LobbyManager::lobbyCreated, Qt::QueuedConnection);
    connect(handler_, &Handler::S_LobbyJoined, this, &LobbyManager::lobbyJoined, Qt::QueuedConnection);

    sWidget = window_->getStackedWidget();
}

void LobbyManager::lobbyCreated(const std::string& code) {
    sWidget->setCurrentIndex(1);
    std::cerr << "LOBBYCODE: " << code << '\n';
}

void LobbyManager::lobbyJoined() {
    sWidget->setCurrentIndex(1);
    std::cerr << "LOBBY JOINED\n";
}
