#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPointF>

static const int CELL_SIZE = 20;

class ShipItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    ShipItem(int size);

    QPointF oldPosition()
    {
        return oldPos_;
    }

    bool isHorizontal()
    {
        return horizontal_;
    }

    int size()
    {
        return shipSize_;
    }

    void setDirection(bool horizontal);
    void rotate();

    void restoreState();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    int shipSize_;
    bool horizontal_ = true;
    QPointF oldPos_;
    bool oldHorizontal_;

signals:
    void placed(ShipItem *ship, int row, int col, int size, bool horizontal);
    void requestRotate(ShipItem *ship);
};

#endif // SHIPITEM_H
