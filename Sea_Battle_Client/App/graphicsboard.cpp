#include "graphicsboard.h"

static const int CELL_SIZE = 20;
static const int OFFSET = 25;

GraphicsBoard::GraphicsBoard(QGraphicsScene *scene) : QObject(scene), scene_(scene) {}

void GraphicsBoard::draw()
{
    scene_->setSceneRect(0, 0, OFFSET + 10 * CELL_SIZE, OFFSET + 10 * CELL_SIZE);

    QPen pen(Qt::black);


    for(int row = 0; row < 10; row++)
    {
        for(int col = 0; col < 10; col++)
        {
            cells_[row][col] = scene_->addRect(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE, CELL_SIZE, CELL_SIZE, pen);

            cells_[row][col]->setBrush(QColor(0,0,139));

            QPixmap pixmap(":/field/images/wave.png");

            QGraphicsPixmapItem* item = scene_->addPixmap(pixmap.scaled(CELL_SIZE,CELL_SIZE));

            item->setPos(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE);

            images_[row][col] = item;
        }
    }

    addCoord();
}

void GraphicsBoard::addCoord()
{
    QString letters = "ABCDEFGHIJ";


    for(int col = 0; col < 10; col++)
    {
        QGraphicsTextItem* text = scene_->addText(QString(letters[col]));
        text->setPos(OFFSET + col * CELL_SIZE + 5, 0);
    }

    for(int row = 0; row < 10; row++)
    {
        QGraphicsTextItem* text = scene_->addText(QString::number(row + 1));
        text->setPos(5, OFFSET + row * CELL_SIZE);
    }
}
