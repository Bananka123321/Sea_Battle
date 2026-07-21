#include "board.h"

#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>

PlacementBoard::PlacementBoard(QGraphicsScene *scene) : GraphicsBoard(scene)
{
    clearField();
}

void PlacementBoard::clear()
{
    ships_.clear();

    clearField();
}

void PlacementBoard::clearField()
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
                {
                    return false;
                }
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

}

void PlacementBoard::rebuildCells(ShipItem* ignoreShip)
{
    clearField();

    for(auto &ship : ships_)
    {
        if(ship.item == ignoreShip)
        {
            continue;
        }

        if(ship.horizontal)
        {
            for(int i = 0; i < ship.size; i++)
            {
                shipCells_[ship.row][ship.col+i] = 1;
            }
        }
        else
        {
            for(int i = 0; i < ship.size; i++)
            {
                shipCells_[ship.row+i][ship.col] = 1;
            }
        }
    }
}

void PlacementBoard::addShip(ShipItem* item, int row, int col, int size, bool horizontal)
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        return;
    }

    for(auto &ship : ships_)
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

    ships_.append(
        {
            item,
            row,
            col,
            size,
            horizontal
        });
}

void PlacementBoard::removeShip(ShipItem *item)
{
    for (int i = 0; i < ships_.size(); i++)
    {
        if (ships_[i].item == item)
        {
            ships_.removeAt(i);
            return;
        }
    }
}

void PlacementBoard::showForbiddenZones(ShipItem *ignoreShip)
{
    for (int r = 0; r < 10; r++)
    {
        for (int c = 0; c < 10; c++)
        {
            cells_[r][c]->setBrush(QColor(0,0,139));
        }
    }

    for (const auto &ship : ships_)
    {
        if (ship.item == ignoreShip)
        {
            continue;
        }

        int r1 = ship.row - 1;
        int c1 = ship.col - 1;

        int r2 = ship.horizontal
                     ? ship.row + 1
                     : ship.row + ship.size;

        int c2 = ship.horizontal
                     ? ship.col + ship.size
                     : ship.col + 1;

        for (int r = r1; r <= r2; r++)
        {
            for (int c = c1; c <= c2; c++)
            {
                if (r < 0 || r >= 10 || c < 0 || c >= 10)
                {
                    continue;
                }

                cells_[r][c]->setBrush(QColor(255,0,0,120));
            }
        }
    }
}

void PlacementBoard::hideForbiddenZones()
{
    for (int r = 0; r < 10; r++)
    {
        for (int c = 0; c < 10; c++)
        {
            cells_[r][c]->setBrush(QColor(0,0,139));
        }
    }
}

void PlacementBoard::registerShip(ShipItem *ship)
{
    allShips_.append(ship);
}

void PlacementBoard::randomPlacement()
{
    clearField();
    ships_.clear();

    for (ShipItem *ship : allShips_)
    {
        while (true)
        {
            bool horizontal = QRandomGenerator::global()->bounded(2);

            int row = QRandomGenerator::global()->bounded(10);
            int col = QRandomGenerator::global()->bounded(10);

            if (!canPlaceShip(nullptr, row, col, ship->size(), horizontal))
            {
                continue;
            }

            ship->setDirection(horizontal);

            ship->setPos(
                OFFSET + col * CELL_SIZE,
                OFFSET + row * CELL_SIZE
                );

            addShip(ship,
                    row,
                    col,
                    ship->size(),
                    horizontal);

            break;
        }
    }
}

bool PlacementBoard::allShipPlaced()
{
    if (ships_.size() != 10)
    {
        return false;
    }

    for (const auto &ship : ships_)
    {
        if (ship.row < 0 || ship.row > 9)
        {
            return false;
        }
        if (ship.col < 0 || ship.col > 9)
        {
            return false;
        }
    }

    return true;
}

const QList<PlacementBoard::ShipData>& PlacementBoard::getShips()
{
    return ships_;
}

const QList<ShipItem*>& PlacementBoard::getAllShips()
{
    return allShips_;
}
