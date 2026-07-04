#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class ShipItem : public QGraphicsRectItem
{
public:
    ShipItem(int size);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int shipSize_;

};

#endif // SHIPITEM_H
