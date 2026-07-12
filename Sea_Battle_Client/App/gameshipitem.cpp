#include "gameshipitem.h"

#include <QBrush>

static const int CELL_SIZE = 20;

GameShipItem::GameShipItem(int size, bool horizontal)
{
    if (horizontal)
    {
        setRect(0, 0, size * CELL_SIZE, CELL_SIZE);
    }
    else
    {
        setRect(0, 0, CELL_SIZE, size * CELL_SIZE);
    }

    setBrush(QColor(255, 128, 0));
}
