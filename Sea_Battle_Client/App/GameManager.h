#pragma once

#include "Handler.h"
#include "mainwindow.h"

class GameManager : public QObject {
    Q_OBJECT

public:
    GameManager(Handler* handler, MainWindow* window);

private:

private:
    Handler* handler_;
    MainWindow* window_;
};
