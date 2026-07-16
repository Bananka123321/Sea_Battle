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

void ShipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);

    int col = qRound((pos().x() - OFFSET) / CELL_SIZE);
    int row = qRound((pos().y() - OFFSET) / CELL_SIZE);


    if (!(row >= -3 && row <= 12 && col >= -3 && col <= 12))
    {
        restoreState();
        return;
    }

    if (row >= -1 && row <= 10 && col >= -1 && col <= 10)
    {
        emit placed(this, row, col, shipSize_, horizontal_);
        return;
    }

    restoreState();
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

bool isPlacementArea(int row, int col)
{
    return row >= -3 && row <= 12 &&
           col >= -3 && col <= 12;
}
