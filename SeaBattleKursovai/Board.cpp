#include "Board.h"

Board::Board(int r, int c)
    : rows(r), cols(c)
{
    grid.resize(rows, std::vector<Cell>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            grid[i][j] = Cell(i, j);
}

// Не константная версия
Cell& Board::getCell(int row, int col)
{
    return grid[row][col];
}

// Константная версия
const Cell& Board::getCell(int row, int col) const
{
    return grid[row][col];
}

bool Board::placeShip(const std::vector<Cell*>& shipCells)
{
    for (auto* cell : shipCells)
    {
        if (!cell->isEmpty())
            return false;
    }

    for (auto* cell : shipCells)
        cell->setState(CellState::Ship);

    ships.emplace_back(shipCells);
    return true;
}

bool Board::shootAt(int row, int col)
{
    Cell& target = grid[row][col];
    return target.shoot();
}

bool Board::allShipsSunk() const
{
    for (const auto& ship : ships)
        if (!ship.isSunk())
            return false;
    return true;
}

const std::vector<std::vector<Cell>>& Board::getGrid() const
{
    return grid;
}

std::vector<std::vector<int>> Board::getStateGrid() const
{
    std::vector<std::vector<int>> state(grid.size(), std::vector<int>(grid[0].size(), 0));
    for (size_t i = 0; i < grid.size(); ++i)
        for (size_t j = 0; j < grid[i].size(); ++j)
            state[i][j] = static_cast<int>(grid[i][j].getState());
    return state;
}