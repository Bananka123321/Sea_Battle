#include "gameshipitem.h"

#include <QPixmap>

static const int CELL_SIZE = 20;

GameShipItem::GameShipItem(int size, bool horizontal)
    : shipSize_(size),
    horizontal_(horizontal)
{
    for(int i = 0; i < shipSize_; i++)
    {
        auto part = new QGraphicsPixmapItem(this);

        part->setPixmap(
            QPixmap(":/field/images/ship.png")
                .scaled(CELL_SIZE, CELL_SIZE)
            );

        parts_.append(part);
    }

    createShip();
}

QRectF GameShipItem::boundingRect() const
{
    if(horizontal_)
        return QRectF(0,0,shipSize_*CELL_SIZE,CELL_SIZE);

    return QRectF(0,0,CELL_SIZE,shipSize_*CELL_SIZE);
}

void GameShipItem::createShip()
{
    prepareGeometryChange();

    for(int i = 0; i < shipSize_; i++)
    {
        if(horizontal_)
            parts_[i]->setPos(i * CELL_SIZE, 0);
        else
            parts_[i]->setPos(0, i * CELL_SIZE);
    }
}
