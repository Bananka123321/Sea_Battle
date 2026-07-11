#ifndef CLICKABLESCENE_H
#define CLICKABLESCENE_H

#include <QGraphicsScene>

class ClickableScene : public QGraphicsScene
{
    Q_OBJECT

public:

    ClickableScene(QObject *parent = nullptr);

signals:

    void clicked(QPointF pos);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CLICKABLESCENE_H
