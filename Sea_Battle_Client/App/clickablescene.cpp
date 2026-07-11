#include "clickablescene.h"

#include <QGraphicsSceneMouseEvent>

ClickableScene::ClickableScene(QObject *parent) : QGraphicsScene(parent) {}

void ClickableScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(event->scenePos());

    QGraphicsScene::mousePressEvent(event);
}
