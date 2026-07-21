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

    void clear();

    void drawField();
    void drawImage();

    void setCellBorder(int row, int col, const QPen &pen);

    QGraphicsPixmapItem *getImage(int row, int col) const;

protected:

    void addCoord();

    QGraphicsScene *scene_;
    QGraphicsRectItem *cells_[10][10];
    QGraphicsPixmapItem *images_[10][10];
};

#endif // GRAPHICSBOARD_H
