#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPointF>
#include <QtMath>
#include <QBrush>
#include <QColor>
#include <QKeyEvent>

static const int CELL_SIZE = 20;

class ShipItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    ShipItem(int size);

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

signals:
    void placed(ShipItem *ship, int row, int col, int size, bool horizontal);
    void requestRotate(ShipItem *ship);
};
