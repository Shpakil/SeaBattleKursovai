#include "Board.h"


// Конструктор
Board::Board(int r, int c)
    : rows(r), cols(c)
{
    grid.resize(rows, std::vector<Cell>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            grid[i][j] = Cell(i, j);
}

// Получить клетку по координатам
Cell& Board::getCell(int row, int col)
{
    return grid[row][col];
}

// Расстановка корабля
bool Board::placeShip(const std::vector<Cell*>& shipCells)
{
    // Проверяем, что все клетки пусты
    for (auto* cell : shipCells)
    {
        if (!cell->isEmpty())
            return false;
    }

    // Ставим корабль
    for (auto* cell : shipCells)
        cell->setState(CellState::Ship);

    ships.emplace_back(shipCells);
    return true;
}

// Выстрел по клетке
bool Board::shootAt(int row, int col)
{
    Cell& target = grid[row][col];
    return target.shoot();  // shoot() вернёт true если попали
}

// Проверка конца игры
bool Board::allShipsSunk() const
{
    for (const auto& ship : ships)
        if (!ship.isSunk())
            return false;
    return true;
}

// Получить сетку для UI
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