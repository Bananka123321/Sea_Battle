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
    void setCellImage(int row, int col, const QPixmap &pixmap);
    void clearCellImage(int row, int col);
    void shootAtCell(int row, int col, Action type);
    void markCellAsKill(std::vector<std::pair<int,int>> shipCells);
    void showAllShips(std::vector<ShipData> allShips);

private slots:

    void onClick(QPointF pos);

signals:

    void cellClicked(int row, int col);

private:

    void setCellColor(int row, int col, QColor color);
    void setCellColor(int row, int col, QRadialGradient);

};
#endif // GAMEBOARD_H
