#pragma once
#include <vector>
#include "Cell.h"
#include "Ship.h"

class Board
{
private:
    int rows, cols;
    std::vector<std::vector<Cell>> grid;
    std::vector<Ship> ships;

public:
    Board(int r = 10, int c = 10);

    // Не константная версия (для изменения клеток)
    Cell& getCell(int row, int col);

    // Константная версия (для чтения)
    const Cell& getCell(int row, int col) const;

    bool placeShip(const std::vector<Cell*>& shipCells);
    bool shootAt(int row, int col);
    bool allShipsSunk() const;
    const std::vector<std::vector<Cell>>& getGrid() const;
    std::vector<std::vector<int>> getStateGrid() const;
    const std::vector<Ship>& getShips() const { return ships; }
};


