#pragma once

#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QVector>

static const int CELL_SIZE = 20;
static const int OFFSET = 25;

class ShipItem : public QGraphicsObject
{
    Q_OBJECT

public:
    ShipItem(int size);

    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}

    QPointF oldPosition();
    bool isHorizontal();

    int size();

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
    QVector<QGraphicsPixmapItem*> parts_;

    void updateParts();

signals:
    void placed(ShipItem *ship, int row, int col, int size, bool horizontal);
    void requestRotate(ShipItem *ship);
};
