#ifndef GRAPHICSBOARD_H
#define GRAPHICSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

class GraphicsBoard : public QObject
{
    Q_OBJECT

public:
    GraphicsBoard(QGraphicsScene *scene);

    virtual ~GraphicsBoard() = default;

    void draw();

protected:

    void addCoord();

    QGraphicsScene *scene_;
    QGraphicsRectItem *cells_[10][10];
    QGraphicsPixmapItem *images_[10][10];
};

#endif // GRAPHICSBOARD_H
