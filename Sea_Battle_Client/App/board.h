#ifndef BOARD_H
#define BOARD_H

#include <QList>

#include "shipitem.h"
#include "graphicsboard.h"

class PlacementBoard : public GraphicsBoard
{
public:
    PlacementBoard(QGraphicsScene *scene);

    bool canPlaceShip(ShipItem *ship,int row, int col, int size, bool horizontal);
    void placeShip(int row, int col, int size, bool horizontal);
    void clear();
    void rebuildCells(ShipItem* ignoreShip = nullptr);
    void addShip(ShipItem* item, int row, int col, int size, bool horizontal);
    void updateBoardView();

    struct ShipData
    {
        ShipItem* item;
        int row;
        int col;
        int size;
        bool horizontal;
    };

    const QList<ShipData>& getShip();

private:
    int shipCells_[10][10];
    QList<ShipData> ships_;
};

#endif // BOARD_H
