#include "shipitem.h"
#include <QtMath>
#include <QBrush>

static const int CELL_SIZE = 40;

ShipItem::ShipItem(int size) : shipSize_(size)
{
    setRect(0, 0, size * CELL_SIZE, CELL_SIZE);

    setBrush(Qt::darkGray);

    setFlags(
        QGraphicsItem::ItemIsMovable |
        QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges
        );
}

void ShipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);

    QPointF p = pos();

    int col = qRound(p.x() / CELL_SIZE);
    int row = qRound(p.y() / CELL_SIZE);

    col = qBound(0, col, 9);
    row = qBound(0, row, 9);

    setPos(col * CELL_SIZE, row * CELL_SIZE);
}

