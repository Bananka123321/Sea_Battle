#include "gameboard.h"

#include <QPen>

GameBoard::GameBoard(QGraphicsScene *scene) : scene_(scene)
{
    ClickableScene* clickable = dynamic_cast<ClickableScene*>(scene_);
    if (clickable)
    {
        connect(clickable, &ClickableScene::clicked, this, &GameBoard::onClick);
    }
}

void GameBoard::draw()
{
    scene_->setSceneRect(0, 0, OFFSET + 10 * CELL_SIZE, OFFSET + 10 * CELL_SIZE);

    QPen pen(Qt::black);


    for(int row = 0; row < 10; row++)
    {
        for(int col = 0; col < 10; col++)
        {
            scene_->addRect(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE, CELL_SIZE, CELL_SIZE, pen);
        }
    }

    addCoord();
}

void GameBoard::addCoord()
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

void GameBoard::addShip(int row, int col, int size, bool horizontal)
{
    GameShipItem *ship = new GameShipItem(size, horizontal);

    ship->setPos(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE);

    scene_->addItem(ship);
}

void GameBoard::onClick(QPointF pos)
{
    int row = (pos.y() - OFFSET) / CELL_SIZE;
    int col = (pos.x() - OFFSET) / CELL_SIZE;

    if (row < 0 || row >= 10)
    {
        return;
    }

    if (col < 0 || col >= 10)
    {
        return;
    }

    emit cellClicked(row, col);
}
