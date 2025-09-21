#include "Ship.h"


Ship::Ship(const std::vector<Cell*>& shipCells)
    : cells(shipCells)
{
}

// ��������, �������� �� �������
bool Ship::isSunk() const
{
    for (auto* cell : cells)
    {
        if (cell->getState() != CellState::Hit)
            return false;  
    }
    return true;
}

// �������� ������ �������
const std::vector<Cell*>& Ship::getCells() const
{
    return cells;
}

