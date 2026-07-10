#include "GameManager.h"

GameManager::GameManager(Handler* handler, MainWindow* window) : handler_(handler), window_(window) {
    connect(handler_, &Handler::S_LobbyCreated, this, [this](const std::string& code){

    });
}

