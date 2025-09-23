#include "Ship.h"

Ship::Ship(const std::vector<Cell*>& shipCells)
    : cells(shipCells)
{
}

// Проверка, потоплен ли корабль
bool Ship::isSunk() const
{
    for (auto* cell : cells)
    {
        if (cell->getState() != CellState::Hit)
            return false;  
    }
    return true;
}


