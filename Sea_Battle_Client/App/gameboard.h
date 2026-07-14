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

static const int OFFSET = 25;

enum class Action
{
    Hit,
    Miss
};

class GameBoard : public QObject
{
    Q_OBJECT

public:

    GameBoard(QGraphicsScene *scene);

    void draw();
    void addShip(int row, int col, int size, bool horizontal);
    void setCellImage(int row, int col, QString path);
    void shootAtCell(int row, int col, Action type);

private slots:

    void onClick(QPointF pos);

signals:

    void cellClicked(int row, int col);

private:

    QGraphicsScene* scene_;
    QGraphicsRectItem* cells_[10][10];
    QGraphicsPixmapItem* images_[10][10];

    void addCoord();
    void setCellColor(int row, int col, QColor color);
    void setCellColor(int row, int col, QRadialGradient);

};
#endif // GAMEBOARD_H
