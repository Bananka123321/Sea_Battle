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
    void shootResult(int row, int column, int status, bool yourTurn, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells);

private:
    Handler* handler_;
    MainWindow* window_;
    Ui::MainWindow* UI_;
};
