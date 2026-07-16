#include "board.h"
#include <QDebug>

PlacementBoard::PlacementBoard(QGraphicsScene *scene) : GraphicsBoard(scene)
{
    clear();
}

void PlacementBoard::clear()
{
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            shipCells_[row][col] = 0;
        }
    }
}

bool PlacementBoard::canPlaceShip(ShipItem *ship, int row, int col, int size, bool horizontal)
{
    rebuildCells(ship);

    if (horizontal)
    {
        if (col + size > 10)
        {
            return false;
        }
    }
    else
    {
        if (row + size > 10)
        {
            return false;
        }
    }

    for (int r = row - 1; r <= row + 1 + (horizontal ? 0 : size - 1); r++)
    {
        for (int c = col - 1; c <= col + 1 + (horizontal ? size - 1 : 0); c++)
        {
            if (r >= 0 && r < 10 && c >= 0 && c < 10)
            {
                if (shipCells_[r][c] == 1)
                    return false;
            }
        }
    }

    return true;
}

void PlacementBoard::placeShip(int row, int col, int size, bool horizontal)
{
    if (horizontal)
    {
        for(int i = 0; i < size; i++)
        {
            shipCells_[row][col + i] = 1;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            shipCells_[row + i][col] = 1;
        }
    }
    qDebug() << shipCells_[row][col];

}

void PlacementBoard::rebuildCells(ShipItem* ignoreShip)
{
    clear();

    for(auto &ship : ships_)
    {
        if(ship.item == ignoreShip)
            continue;

        if(ship.horizontal)
        {
            for(int i = 0; i < ship.size; i++)
                shipCells_[ship.row][ship.col+i] = 1;
        }
        else
        {
            for(int i = 0; i < ship.size; i++)
                shipCells_[ship.row+i][ship.col] = 1;
        }
    }
}

void PlacementBoard::addShip(ShipItem* item, int row, int col, int size, bool horizontal)
{
    for(auto &ship : ships_)
    {
        if(ship.item == item)
        {
            ship.row = row;
            ship.col = col;
            ship.size = size;
            ship.horizontal = horizontal;
            updateBoardView();
            return;
        }
    }

    ships_.append(
        {
            item,
            row,
            col,
            size,
            horizontal
        }
        );

    updateBoardView();
}

void PlacementBoard::updateBoardView()
{
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            images_[row][col]->setVisible(true);
        }
    }

    for (const auto &ship : ships_)
    {
        for (int i = 0; i < ship.size; i++)
        {
            int r = ship.horizontal ? ship.row : ship.row + i;
            int c = ship.horizontal ? ship.col + i : ship.col;

            images_[r][c]->setVisible(false);
        }
    }
}

const QList<PlacementBoard::ShipData>& PlacementBoard::getShip()
{
    return ships_;
}
