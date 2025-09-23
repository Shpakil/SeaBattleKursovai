#pragma once
#include <vector>
#include "Cell.h"

class Ship
{
private:
    std::vector<Cell*> cells;

public:
    Ship(const std::vector<Cell*>& shipCells);

    bool isSunk() const;
    const std::vector<Cell*>& getCells() const { return cells; } 
};
