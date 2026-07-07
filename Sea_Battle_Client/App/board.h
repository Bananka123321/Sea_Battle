#ifndef BOARD_H
#define BOARD_H

#include "shipitem.h"

class board
{
public:
    board();

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

private:
    int cells[10][10];
    QList<ShipData> ships;
};

#endif // BOARD_H
