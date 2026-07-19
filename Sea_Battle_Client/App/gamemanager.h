#pragma once
#include <QObject>

#include "Handler.h"
#include "mainwindow.h"

class gameManager : public QObject{
    Q_OBJECT

public :
    gameManager(Handler* handler, MainWindow* window);

private:
    void gameStarted(bool yourTurn);
    void shootResult(int row, int column, int result, bool yourTurn);

private:
    Handler* handler_;
    MainWindow* window_;
    Ui::MainWindow* UI_;
};
