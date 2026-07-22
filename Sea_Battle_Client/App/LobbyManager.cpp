#include "LobbyManager.h"

LobbyManager::LobbyManager(Handler* handler, MainWindow* window) : handler_(handler), window_(window) {
    connect(handler_, &Handler::S_LobbyCreated, this, &LobbyManager::lobbyCreated, Qt::QueuedConnection);
    connect(handler_, &Handler::S_LobbyJoined, this, &LobbyManager::lobbyJoined, Qt::QueuedConnection);
    connect(handler_, &Handler::S_PlayerJoined, this, &LobbyManager::playerJoined, Qt::QueuedConnection);
    connect(handler_, &Handler::S_PlayerLeft, this, &LobbyManager::playerLeft, Qt::QueuedConnection);
    connect(handler_, &Handler::S_EnemyReady, this, &LobbyManager::playerReady, Qt::QueuedConnection);
    connect(window_, &MainWindow::revengeRequest, this, &LobbyManager::playerReady, Qt::QueuedConnection);

    UI_ = window_->getUI();
}

void LobbyManager::lobbyCreated(const std::string& code) {
    UI_->stackedWidget->setCurrentIndex(1);
    window_->setPlayerState(Ui::PlayerState::NOT_PLAYER);
    UI_->graphicsView->fitInView(window_->getPlaceScene()->sceneRect(), Qt::KeepAspectRatio);
    UI_->graphicsView->scale(0.8, 0.8);
    UI_->CodeRoomLabel->setText(QString::fromStdString(code));
}

void LobbyManager::lobbyJoined(bool success, const std::string& username) {
    UI_->stackedWidget->setCurrentIndex(1);
    UI_->graphicsView->fitInView(window_->getPlaceScene()->sceneRect(), Qt::KeepAspectRatio);
    UI_->graphicsView->scale(0.8, 0.8);
    success ? window_->setPlayerState(Ui::PlayerState::PLAYER_READY) : window_->setPlayerState(Ui::PlayerState::PLAYER_NOT_READY);
    UI_->NameOPLabel->setText(QString::fromStdString(username));
    UI_->NameOPInGameLabel->setText(QString::fromStdString(username));
}

void LobbyManager::playerJoined(const std::string& username) {
    window_->setPlayerState(Ui::PlayerState::PLAYER_NOT_READY);
    UI_->NameOPLabel->setText(QString::fromStdString(username));
    UI_->NameOPInGameLabel->setText(QString::fromStdString(username));
}

void LobbyManager::playerLeft() {
    window_->setPlayerState(Ui::PlayerState::NOT_PLAYER);
    UI_->NameOPLabel->setText("???");
    UI_->NameOPInGameLabel->setText("???");
}

void LobbyManager::playerReady(bool success = false) {
    success ? window_->setPlayerState(Ui::PlayerState::PLAYER_READY) : window_->setPlayerState(Ui::PlayerState::PLAYER_NOT_READY);
}
