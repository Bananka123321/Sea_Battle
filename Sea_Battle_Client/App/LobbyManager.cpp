#include "LobbyManager.h"

LobbyManager::LobbyManager(Handler* handler, MainWindow* window) : handler_(handler), window_(window) {
    connect(handler_, &Handler::S_LobbyCreated, this, &LobbyManager::lobbyCreated, Qt::QueuedConnection);
    connect(handler_, &Handler::S_LobbyJoined, this, &LobbyManager::lobbyJoined, Qt::QueuedConnection);
    connect(handler_, &Handler::S_PlayerJoined, this, &LobbyManager::playerJoined, Qt::QueuedConnection);
    connect(handler_, &Handler::S_PlayerLeft, this, &LobbyManager::playerLeft, Qt::QueuedConnection);
    connect(handler_, &Handler::S_EnemyReady, this, &LobbyManager::playerReady, Qt::QueuedConnection);

    UI_ = window_->getUI();
}

void LobbyManager::lobbyCreated(const std::string& code) {
    UI_->stackedWidget->setCurrentIndex(1);
    UI_->CodeRoomLabel->setText(QString::fromStdString(code));
}

void LobbyManager::lobbyJoined(bool success) {
    UI_->stackedWidget->setCurrentIndex(1);
    success ? window_->setPlayerState(Ui::PlayerState::PLAYER_READY) : window_->setPlayerState(Ui::PlayerState::PLAYER_NOT_READY);
}

void LobbyManager::playerJoined(const std::string& username) {
    window_->setPlayerState(Ui::PlayerState::PLAYER_NOT_READY);
    // UI_->PlayerName->setText(QString::fromStdString(username));//Для смены ника в чате
}

void LobbyManager::playerLeft() {
    window_->setPlayerState(Ui::PlayerState::NOT_PLAYER);
    // UI_->PlayerName->setText("");//Для смены ника в чате
}

void LobbyManager::playerReady(bool success) {
    success ? window_->setPlayerState(Ui::PlayerState::PLAYER_READY) : window_->setPlayerState(Ui::PlayerState::PLAYER_NOT_READY);
}
