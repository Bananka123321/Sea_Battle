#ifndef GAMESHIPITEM_H
#define GAMESHIPITEM_H

#include <QGraphicsRectItem>

class GameShipItem : public QGraphicsRectItem
{
public:
    GameShipItem(int size, bool horizontal);
};

#endif // GAMESHIPITEM_H
