#include "shipitem.h"


ShipItem::ShipItem(int size) : shipSize_(size)
{
    setRect(0, 0, size * CELL_SIZE, CELL_SIZE);


    setBrush(QColor(255,128,0));

    setFlags(
        QGraphicsItem::ItemIsMovable |
        QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemIsFocusable
        );
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
    QGraphicsRectItem::mousePressEvent(event);
}

void ShipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsRectItem::mouseReleaseEvent(event);

    int col = qRound(pos().x() / CELL_SIZE);
    int row = qRound(pos().y() / CELL_SIZE);

    if(horizontal_)    {
        col = qBound(0, col, 10 - shipSize_);
        row = qBound(0, row, 9);
    } else {
        col = qBound(0, col, 9);
        row = qBound(0, row, 10 - shipSize_);
    }

    setPos(col * CELL_SIZE, row * CELL_SIZE);

    emit placed(this, row, col, shipSize_, horizontal_);
}

void ShipItem::keyPressEvent(QKeyEvent *event) {
    if (event->key() ==  Qt::Key_R || event->text().toLower() == "к")
        emit requestRotate(this);

    int col = qRound(pos().x() / CELL_SIZE);
    int row = qRound(pos().y() / CELL_SIZE);

    if(horizontal_) {
        col = qBound(0, col, 10 - shipSize_);
        row = qBound(0, row, 9);
    } else {
        col = qBound(0, col, 9);
        row = qBound(0, row, 10 - shipSize_);
    }

    setPos(col * CELL_SIZE, row * CELL_SIZE);
}

void ShipItem::setDirection(bool horizontal) {
    horizontal_ = horizontal;

    if (horizontal_)
        setRect(0,0,shipSize_ * CELL_SIZE, CELL_SIZE);
    else
        setRect(0,0,CELL_SIZE, shipSize_ * CELL_SIZE);
}

void ShipItem::rotate() {
    horizontal_ = !horizontal_;

    if (horizontal_)
        setRect(0, 0, shipSize_ * CELL_SIZE, CELL_SIZE);
    else
        setRect(0, 0, CELL_SIZE, shipSize_ * CELL_SIZE);
}

void ShipItem::restoreState() {
    setDirection(oldHorizontal_);
    setPos(oldPos_);
}
