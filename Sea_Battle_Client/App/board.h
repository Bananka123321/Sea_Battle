#ifndef BOARD_H
#define BOARD_H

#include <QList>

#include "shipitem.h"
#include "graphicsboard.h"

class PlacementBoard : public GraphicsBoard
{
public:
    PlacementBoard(QGraphicsScene *scene);

    void clear();

    bool canPlaceShip(ShipItem *ship,int row, int col, int size, bool horizontal);
    void placeShip(int row, int col, int size, bool horizontal);
    void clearField();
    void rebuildCells(ShipItem* ignoreShip = nullptr);
    void addShip(ShipItem* item, int row, int col, int size, bool horizontal);
    void updateBoardView();
    void removeShip(ShipItem *item);
    void showForbiddenZones(ShipItem *ignoreShip);
    void hideForbiddenZones();

    void registerShip(ShipItem *ship);
    void randomPlacement();

    bool allShipPlaced();


    struct ShipData
    {
        ShipItem* item;
        int row;
        int col;
        int size;
        bool horizontal;
    };

    const QList<ShipData>& getShips();
    const QList<ShipItem*>& getAllShips();

private:
    int shipCells_[10][10];
    QList<ShipItem*> allShips_;
    QList<ShipData> ships_;
};

#endif // BOARD_H
