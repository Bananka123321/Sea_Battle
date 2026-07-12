#ifndef BOARD_H
#define BOARD_H

#include <QList>

#include "shipitem.h"

class Board
{
public:
    Board();

    bool canPlaceShip(ShipItem *ship,int row, int col, int size, bool horizontal);
    void placeShip(int row, int col, int size, bool horizontal);
    void clear();
    void rebuildCells(ShipItem* ignoreShip = nullptr);
    void addShip(ShipItem* item, int row, int col, int size, bool horizontal);

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
    int cells[10][10];
    QList<ShipData> ships;
};

#endif // BOARD_H
