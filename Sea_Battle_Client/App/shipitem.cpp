#include "shipitem.h"


ShipItem::ShipItem(int size) : shipSize_(size)
{
    for(int i = 0; i < shipSize_; i++)
    {
        auto part = new QGraphicsPixmapItem(QPixmap(":/field/images/ship.png").scaled(CELL_SIZE, CELL_SIZE), this);
        parts_.append(part);
    }

    updateParts();

    setFlag(ItemIsMovable);
    setFlag(ItemIsFocusable);
}

QRectF ShipItem::boundingRect() const
{
    if(horizontal_)
    {
        return QRectF(0,0,shipSize_*CELL_SIZE,CELL_SIZE);
    }

    return QRectF(0,0,CELL_SIZE,shipSize_*CELL_SIZE);
}

void ShipItem::updateParts()
{
    prepareGeometryChange();

    for(int i=0;i<shipSize_;i++)
    {
        if(horizontal_)
        {
            parts_[i]->setPos(i*CELL_SIZE,0);
        }
        else
        {
            parts_[i]->setPos(0,i*CELL_SIZE);
        }
    }

    update();
}

QPointF ShipItem::oldPosition() {
    return oldPos_;
}

bool ShipItem::isHorizontal() {
    return horizontal_;
}

int ShipItem::size() {
    return shipSize_;
}

void ShipItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    oldPos_ = pos();
    oldHorizontal_ = horizontal_;
    QGraphicsObject::mousePressEvent(event);
}

void ShipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsObject::mouseReleaseEvent(event);

    int col = qRound((pos().x() - OFFSET) / CELL_SIZE);
    int row = qRound((pos().y() - OFFSET) / CELL_SIZE);

    if(horizontal_)
    {
        col = qBound(0, col, 10 - shipSize_);
        row = qBound(0, row, 9);
    } else
    {
        col = qBound(0, col, 9);
        row = qBound(0, row, 10 - shipSize_);
    }

    setPos(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE);

    emit placed(this, row, col, shipSize_, horizontal_);
}

void ShipItem::keyPressEvent(QKeyEvent *event) {
    if (event->key() ==  Qt::Key_R || event->text().toLower() == "к")
    {
        emit requestRotate(this);
    }
    QGraphicsObject::keyPressEvent(event);
}

void ShipItem::setDirection(bool horizontal) {
    horizontal_ = horizontal;
    updateParts();
}

void ShipItem::rotate() {
    horizontal_ = !horizontal_;
    updateParts();
}

void ShipItem::restoreState() {
    setDirection(oldHorizontal_);
    setPos(oldPos_);
}
