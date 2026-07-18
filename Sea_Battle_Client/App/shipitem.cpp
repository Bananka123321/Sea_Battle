#include "shipitem.h"


ShipItem::ShipItem(int size) : shipSize_(size)
{
    qDeleteAll(parts_);
    parts_.clear();

    setFlag(ItemIsMovable);
    setFlag(ItemIsFocusable);

    if (shipSize_ == 1)
    {
        QGraphicsPixmapItem *part = new QGraphicsPixmapItem(this);
        part->setPixmap(QPixmap(":/ships/single_H.png").scaled(CELL_SIZE, CELL_SIZE));

        parts_.append(part);
        updateParts();
        return;
    }

    for (int i = 0; i < shipSize_; i++)
    {
        QString path;

        if (i == 0)
        {
            path = ":/ships/tail_H_ship.png";
        }
        else if (i == size - 1)
        {
            path = ":/ships/head_H_ship.png";
        }
        else if (i == 2)
        {
            path = ":/ships/middle_H34_ship.png";
        }
        else
        {
            path = ":/ships/middle_H4_ship.png";
        }

        QGraphicsPixmapItem *part = new QGraphicsPixmapItem(this);
        part->setPixmap(QPixmap(path).scaled(CELL_SIZE, CELL_SIZE));
        part->setPos(i * CELL_SIZE, 0);

        parts_.append(part);
    }

    updateParts();
}

QRectF ShipItem::boundingRect() const
{
    if (horizontal_)
    {
        return QRectF(0, 0, shipSize_ * CELL_SIZE, CELL_SIZE);
    }

    return QRectF(0, 0, CELL_SIZE, shipSize_ * CELL_SIZE);
}

QPainterPath ShipItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void ShipItem::updateParts()
{
    prepareGeometryChange();

    for (int i = 0; i < shipSize_; i++)
    {
        QGraphicsPixmapItem *part = parts_[i];

        part->setTransformOriginPoint(CELL_SIZE / 2.0,CELL_SIZE / 2.0);

        if (horizontal_)
        {
            part->setRotation(0);
            part->setPos(i * CELL_SIZE, 0);
        }
        else
        {
            part->setRotation(90);
            part->setPos(0, i * CELL_SIZE);
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
    emit dragStarted(this);
}

void ShipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);

    int col = qRound((pos().x() - OFFSET) / CELL_SIZE);
    int row = qRound((pos().y() - OFFSET) / CELL_SIZE);

    if (row >= -1 && row <= 10 && col >= -1 && col <= 10)
    {
        emit placed(this, row, col, shipSize_, horizontal_);
        emit dragFinished(this);
        return;
    }

    emit removedFromBoard(this);
    emit dragFinished(this);
    returnSpawn();
}

void ShipItem::keyPressEvent(QKeyEvent *event) {
    if (!hasFocus())
    {
        QGraphicsObject::keyPressEvent(event);
        return;
    }

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
    clearFocus();
}

void ShipItem::setSpawnPos(QPointF pos)
{
    spawnPos_ = pos;
}

void ShipItem::returnSpawn()
{
    setPos(spawnPos_);
    setDirection(true);
    clearFocus();
}

bool isPlacementArea(int row, int col)
{
    return row >= -3 && row <= 12 &&
           col >= -3 && col <= 12;
}
