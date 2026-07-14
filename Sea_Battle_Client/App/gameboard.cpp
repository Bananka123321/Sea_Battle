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
        setCellImage(row, col, ":/field/images/miss.png");
    }
    else
    {
        setCellImage(row, col, ":/field/images/hit.png");

        QRadialGradient gradhit(QPointF(0.5, 0.5), 0.7, QPointF(0.5, 0.5));

        gradhit.setCoordinateMode(QGradient::ObjectBoundingMode);


        gradhit.setColorAt(0.0, QColor(255,0,0,255));
        gradhit.setColorAt(1.0, QColor(0,0,139, 0));

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
