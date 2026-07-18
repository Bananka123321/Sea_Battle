#include "gameboard.h"

#include <QPen>
#include <QRandomGenerator>

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

    images_[row][col]->setPixmap(QPixmap(path).scaled(CELL_SIZE, CELL_SIZE));
}

void GameBoard::setCellImage(int row, int col, const QPixmap &pixmap)
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        return;
    }

    images_[row][col]->setPixmap(pixmap.scaled(CELL_SIZE, CELL_SIZE));
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
        QPixmap pix(":/ships/hit_ship.png");

        QTransform transform;
        transform.rotate(QRandomGenerator::global()->bounded(4) * 90);

        pix = pix.transformed(transform, Qt::SmoothTransformation);

        setCellImage(row, col, pix);

        QRadialGradient gradhit(QPointF(0.5, 0.5), 0.6, QPointF(0.5, 0.5));

        gradhit.setCoordinateMode(QGradient::ObjectBoundingMode);

        gradhit.setColorAt(0.0, QColor(255,0,0,255));
        gradhit.setColorAt(1.0, QColor(0,0,139,255));

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
