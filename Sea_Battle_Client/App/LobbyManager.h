#pragma once
#include <QObject>

#include "Handler.h"
#include "mainwindow.h"

class LobbyManager : public QObject{
    Q_OBJECT

public:
    LobbyManager(Handler* handler, MainWindow* window);

private:
    void lobbyCreated(const std::string& code);
    void lobbyJoined();

private:
    Handler* handler_;
    MainWindow* window_;
    QStackedWidget* sWidget;
};
