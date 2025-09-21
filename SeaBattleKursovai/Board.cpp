#include "Board.h"


// �����������
Board::Board(int r, int c)
    : rows(r), cols(c)
{
    grid.resize(rows, std::vector<Cell>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            grid[i][j] = Cell(i, j);
}

// �������� ������ �� �����������
Cell& Board::getCell(int row, int col)
{
    return grid[row][col];
}

// ����������� �������
bool Board::placeShip(const std::vector<Cell*>& shipCells)
{
    // ���������, ��� ��� ������ �����
    for (auto* cell : shipCells)
    {
        if (!cell->isEmpty())
            return false;
    }

    // ������ �������
    for (auto* cell : shipCells)
        cell->setState(CellState::Ship);

    ships.emplace_back(shipCells);
    return true;
}

// ������� �� ������
bool Board::shootAt(int row, int col)
{
    Cell& target = grid[row][col];
    return target.shoot();  // shoot() ����� true ���� ������
}

// �������� ����� ����
bool Board::allShipsSunk() const
{
    for (const auto& ship : ships)
        if (!ship.isSunk())
            return false;
    return true;
}

// �������� ����� ��� UI
const std::vector<std::vector<Cell>>& Board::getGrid() const
{
    return grid;
}
