#include "gamemanager.h"

gameManager::gameManager(Handler* handler, MainWindow* window) : handler_(handler), window_(window) {
    connect(handler_, &Handler::S_GameStarted, this, &gameManager::gameStarted, Qt::QueuedConnection);
    connect(handler_, &Handler::S_ShotResult, this, &gameManager::shootResult, Qt::QueuedConnection);

    UI_ = window_->getUI();
}

void gameManager::gameStarted(bool yourTurn) {
    UI_->stackedWidget->setCurrentIndex(2);
    window_->setYourTurn(yourTurn);
}

void gameManager::shootResult(int row, int column, int status, bool yourTurn, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells) {
    if (window_->getYourTurn()) {
        window_->shootResultEnemy(row, column, status, shipSunk, shipCells);
    } else {
        window_->shootResultMe(row, column, status, shipSunk, shipCells);
    }

    window_->setYourTurn(yourTurn);
}
