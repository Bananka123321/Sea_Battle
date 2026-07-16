#ifndef GAMESHIPITEM_H
#define GAMESHIPITEM_H

#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QVector>

class GameShipItem : public QGraphicsObject
{
    Q_OBJECT

public:
    GameShipItem(int size, bool horizontal);

    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}

private:
    void createShip();

    int shipSize_;
    bool horizontal_;

    QVector<QGraphicsPixmapItem*> parts_;
};

#endif // GAMESHIPITEM_H
