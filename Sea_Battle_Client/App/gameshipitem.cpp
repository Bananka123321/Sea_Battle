#include "gameshipitem.h"

#include <QPixmap>

static const int CELL_SIZE = 20;

GameShipItem::GameShipItem(int size, bool horizontal)
    : shipSize_(size), horizontal_(horizontal)
{
    if (size == 1)
    {
        auto part = new QGraphicsPixmapItem(QPixmap(":/ships/single_H.png").scaled(CELL_SIZE, CELL_SIZE), this);

        if (!horizontal)
        {
            part->setTransformOriginPoint(CELL_SIZE / 2.0, CELL_SIZE / 2.0);
            part->setRotation(90);
        }
        return;
    }

    for (int i = 0; i < size; i++)
    {
        QString path;

        if (i == 0)
        {
            path = ":/ships/tail_H_ship.png";
        }
        else if (i == size - 1)
        {
            path = ":/ships/head_H_ship.png";
        }
        else if (i == 2)
        {
            path = ":/ships/middle_H34_ship.png";
        }
        else
        {
            path = ":/ships/middle_H4_ship.png";
        }

        auto part = new QGraphicsPixmapItem(QPixmap(path).scaled(CELL_SIZE, CELL_SIZE), this);

        if (horizontal)
        {
            part->setPos(i * CELL_SIZE, 0);
        }
        else
        {
            part->setTransformOriginPoint(CELL_SIZE / 2.0, CELL_SIZE / 2.0);
            part->setRotation(90);
            part->setPos(0, i * CELL_SIZE);
        }
    }
}

QRectF GameShipItem::boundingRect() const
{
    if (horizontal_)
    {
        return QRectF(0, 0, shipSize_ * CELL_SIZE, CELL_SIZE);
    }
    return QRectF(0, 0, CELL_SIZE, shipSize_ * CELL_SIZE);
}

void GameShipItem::createShip()
{
    prepareGeometryChange();

    for(int i = 0; i < shipSize_; i++)
    {
        if(horizontal_)
        {
            parts_[i]->setPos(i * CELL_SIZE, 0);
        }
        else
        {
            parts_[i]->setPos(0, i * CELL_SIZE);
        }
    }
}
