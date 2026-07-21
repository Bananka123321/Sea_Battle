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
    qDebug() << row << col << size << horizontal;
    for (int i = 0; i < size; i++)
    {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;

        QString path;

        if (size == 1)
        {
            path = ":/ships/single_H.png";
        }
        else if (i == 0)
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

        QPixmap pix(path);

        if (!horizontal)
        {
            QTransform transform;
            transform.rotate(90);
            pix = pix.transformed(transform, Qt::SmoothTransformation);
        }

        images_[r][c]->setPixmap(
            pix.scaled(CELL_SIZE, CELL_SIZE,
                       Qt::KeepAspectRatio,
                       Qt::SmoothTransformation));
    }
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

void GameBoard::clearCellImage(int row, int col)
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        return;
    }

    images_[row][col]->setPixmap(QPixmap());
}

void GameBoard::shootAtCell(int row, int col, Action type)
{
    if (type == Action::Miss)
    {
        clearCellImage(row, col);
        images_[row][col]->setData(0,"miss");
    }
    else
    {
        QPixmap pix(":/ships/hit_ship_2.png");
        images_[row][col]->setData(0,"hit");

        QTransform transform;
        transform.rotate(QRandomGenerator::global()->bounded(3) * 90);

        pix = pix.transformed(transform, Qt::SmoothTransformation);

        setCellImage(row, col, pix);
    }
}

void GameBoard::markCellAsKill(std::vector<std::pair<int,int>> shipCells)
{
    std::pair<int,int> cell1 = shipCells[0];
    std::pair<int,int> cell2 = shipCells[shipCells.size() - 1];

    bool horizontal = cell1.first == cell2.first ? true : false;

    int r1 = cell1.first - 1;
    int c1 = cell1.second - 1;

    int r2 = horizontal
                 ? cell1.first + 1
                 : cell2.first + 1;

    int c2 = horizontal
                 ? cell2.second + 1
                 : cell1.second + 1;

    for (int r = r1; r <= r2; r++)
    {
        for (int c = c1; c <= c2; c++)
        {
            if (r < 0 || r >= 10 || c < 0 || c >= 10)
            {
                continue;
            }
            if (horizontal)
            {
                if (cell1.first == r && (c > c1 && c < c2))
                {
                    continue;
                }
            }
            else
            {
                if (cell1.second == c && (r > r1 && r < r2))
                {
                    continue;
                }
            }
            setCellImage(r,c,":/f");
            images_[r][c]->setData(0,"miss");
        }
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
