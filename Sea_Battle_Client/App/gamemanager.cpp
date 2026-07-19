#include "gamemanager.h"

gameManager::gameManager(Handler* handler, MainWindow* window) : handler_(handler), window_(window) {
    connect(handler_, &Handler::S_GameStarted, this, &gameManager::gameStarted, Qt::QueuedConnection);
    connect(handler_, &Handler::S_ShotResult, this, &gameManager::shootResult, Qt::QueuedConnection);

    UI_ = window_->getUI();
}

void gameManager::gameStarted(bool yourTurn) {
    UI_->stackedWidget->setCurrentIndex(1);
    window_->setYourTurn(yourTurn);
}

void gameManager::shootResult(int row, int column, int result, bool yourTurn) {
    window_->setYourTurn(yourTurn);
    window_->shootResult(row, column, result);
}
