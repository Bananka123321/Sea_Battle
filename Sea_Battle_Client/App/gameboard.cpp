#include "gameboard.h"

#include <QPen>

GameBoard::GameBoard(QGraphicsScene *scene) : GraphicsBoard(scene)
{
    ClickableScene* clickable = dynamic_cast<ClickableScene*>(scene_);
    if (clickable)
    {
        connect(clickable, &ClickableScene::clicked, this, &GameBoard::onClick);
    }
}

void GameBoard::addShip(int row, int col, int size, bool horizontal)
{
    for (int i = 0; i < size; i++)
    {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;

        if (images_[r][c])
        {
            images_[r][c]->setVisible(false);
        }
    }

    GameShipItem *ship = new GameShipItem(size, horizontal);

    ship->setPos(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE);

    scene_->addItem(ship);
}

void GameBoard::setCellColor(int row, int col, QColor color)
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        return;
    }

    cells_[row][col]->setBrush(QBrush(color));
}

void GameBoard::setCellColor(int row, int col, QRadialGradient color)
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        return;
    }

    cells_[row][col]->setBrush(QBrush(color));
}

void GameBoard::setCellImage(int row, int col, QString path)
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        return;
    }

    if(images_[row][col])
    {
        scene_->removeItem(images_[row][col]);

        delete images_[row][col];
    }

    QPixmap pixmap(path);

    QGraphicsPixmapItem* item = scene_->addPixmap(pixmap.scaled(CELL_SIZE,CELL_SIZE));

    item->setPos(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE);

    images_[row][col] = item;
}

void GameBoard::shootAtCell(int row, int col, Action type)
{
    if (type == Action::Miss)
    {
        setCellImage(row, col, ":/field/images/missvortex.png");

        QRadialGradient gradhit(QPointF(0.5, 0.5), 0.4, QPointF(0.5, 0.5));

        gradhit.setCoordinateMode(QGradient::ObjectBoundingMode);


        gradhit.setColorAt(0.0, QColor(255,255,255,255));
        gradhit.setColorAt(1.0, QColor(0,0,139, 255));

        // setCellColor(row, col, gradhit);
    }
    else
    {
        setCellImage(row, col, ":/ships/hit_ship.png");

        QRadialGradient gradhit(QPointF(0.5, 0.5), 0.6, QPointF(0.5, 0.5));

        gradhit.setCoordinateMode(QGradient::ObjectBoundingMode);


        gradhit.setColorAt(0.0, QColor(255,0,0,255));
        gradhit.setColorAt(1.0, QColor(0,0,139, 255));

        setCellColor(row, col, gradhit);
    }
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
