#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

#include "board.h"
#include "gameshipitem.h"
#include "clickablescene.h"

static const int OFFSET = 25;

class GameBoard : public QObject
{
    Q_OBJECT

public:

    GameBoard(QGraphicsScene *scene);

    void draw();
    void addShip(int row, int col, int size, bool horizontal);

private slots:

    void onClick(QPointF pos);

signals:

    void cellClicked(int row, int col);

private:

    void addCoord();
    QGraphicsScene* scene_;

};
#endif // GAMEBOARD_H
