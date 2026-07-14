#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QColor>
#include <QGraphicsPixmapItem>

#include "board.h"
#include "gameshipitem.h"
#include "clickablescene.h"
#include "graphicsboard.h"

enum class Action
{
    Hit,
    Miss
};

class GameBoard : public GraphicsBoard
{
    Q_OBJECT

public:

    GameBoard(QGraphicsScene *scene);

    void addShip(int row, int col, int size, bool horizontal);
    void setCellImage(int row, int col, QString path);
    void shootAtCell(int row, int col, Action type);

private slots:

    void onClick(QPointF pos);

signals:

    void cellClicked(int row, int col);

private:

    void setCellColor(int row, int col, QColor color);
    void setCellColor(int row, int col, QRadialGradient);

};
#endif // GAMEBOARD_H
