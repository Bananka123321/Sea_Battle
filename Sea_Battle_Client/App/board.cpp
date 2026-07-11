#include "board.h"
#include <QDebug>

Board::Board()
{
    clear();
}

void Board::clear()
{
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            cells[row][col] = 0;
        }
    }
}

bool Board::canPlaceShip(ShipItem *ship, int row, int col, int size, bool horizontal)
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
                if (cells[r][c] == 1)
                    return false;
            }
        }
    }

    return true;
}

void Board::placeShip(int row, int col, int size, bool horizontal)
{
    if (horizontal)
    {
        for(int i = 0; i < size; i++)
        {
            cells[row][col + i] = 1;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            cells[row + i][col] = 1;
        }
    }
    qDebug() << cells[row][col];
}

void Board::rebuildCells(ShipItem* ignoreShip)
{
    clear();

    for(auto &ship : ships)
    {
        if(ship.item == ignoreShip)
            continue;

        if(ship.horizontal)
        {
            for(int i = 0; i < ship.size; i++)
                cells[ship.row][ship.col+i] = 1;
        }
        else
        {
            for(int i = 0; i < ship.size; i++)
                cells[ship.row+i][ship.col] = 1;
        }
    }
}

void Board::addShip(ShipItem* item, int row, int col, int size, bool horizontal)
{
    for(auto &ship : ships)
    {
        if(ship.item == item)
        {
            ship.row = row;
            ship.col = col;
            ship.size = size;
            ship.horizontal = horizontal;
            return;
        }
    }


    ships.append(
        {
            item,
            row,
            col,
            size,
            horizontal
        }
        );
}

const QList<Board::ShipData>& Board::getShip()
{
    return ships;
}
