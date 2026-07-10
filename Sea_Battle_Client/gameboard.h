#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "board.h"
#include "shipitem.h"

class GameBoard : public QGraphicsView
{
    Q_OBJECT

public:
    enum class Mode
    {
        Placement,
        View,
        Attack
    };


    GameBoard(Mode mode, QWidget *parent = nullptr);

    // board &board();

private:
    void createField();
    void createShips();
    void addShip(ShipItem *ship, int x, int y);

    Mode mode_;

    QGraphicsScene *scene_;
    board board_;
};

#endif // GAMEBOARD_H
