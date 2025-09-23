#include "Board.h"
#include <QDebug>

Board::Board(int r, int c)
    : rows(r), cols(c)
{
    grid.resize(rows, std::vector<Cell>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            grid[i][j] = Cell(i, j);
}

Cell& Board::getCell(int row, int col)
{
    return grid[row][col];
}

const Cell& Board::getCell(int row, int col) const
{
    return grid[row][col];
}

bool Board::placeShip(const std::vector<Cell*>& shipCells)
{
    for (auto* cell : shipCells) {
        if (!cell->isEmpty()) return false;
    }

    for (auto* cell : shipCells) {
        cell->setState(CellState::Ship);
    }

    ships.emplace_back(shipCells);
    return true;
}

bool Board::shootAt(int row, int col)
{
    Cell& target = grid[row][col];
    bool hit = target.shoot();

    if (hit) {
        qDebug() << "Hit at:" << row << col;
        // ������� �������, � ������� ������, � ���������, �������� �� �� ������
        for (auto& ship : ships) {
            for (auto* shipCell : ship.getCells()) {
                if (shipCell->getRow() == row && shipCell->getCol() == col) {
                    if (ship.isSunk()) {
                        qDebug() << "Ship sunk! Marking area around it.";
                        markAreaAroundSunkShip(ship);
                    }
                    break;
                }
            }
        }
    }

    return hit;
}

bool Board::allShipsSunk() const
{
    for (const auto& ship : ships) {
        if (!ship.isSunk()) {
            qDebug() << "Ship not sunk - game continues";
            return false;
        }
    }
    qDebug() << "All ships sunk! Game over.";
    return true;
}
const std::vector<std::vector<Cell>>& Board::getGrid() const
{
    return grid;
}

std::vector<std::vector<int>> Board::getStateGrid() const
{
    std::vector<std::vector<int>> state(grid.size(), std::vector<int>(grid[0].size(), 0));
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            state[i][j] = static_cast<int>(grid[i][j].getState());
        }
    }
    return state;
}

// ����� �����: �������� ������� ������ ������������ �������
void Board::markAreaAroundSunkShip(const Ship& ship)
{
    qDebug() << "Marking area around sunk ship";

    // �������� ��� ������ �������
    const auto& cells = ship.getCells();

    // ��� ������ ������ ������� �������� ������� 3x3 ������ ���
    for (auto* cell : cells) {
        int centerRow = cell->getRow();
        int centerCol = cell->getCol();

        // ������� ��� �������� ������ (������� ������������)
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
            for (int colOffset = -1; colOffset <= 1; colOffset++) {
                int checkRow = centerRow + rowOffset;
                int checkCol = centerCol + colOffset;

                // ���������, ��� ������ � �������� �����
                if (checkRow >= 0 && checkRow < rows && checkCol >= 0 && checkCol < cols) {
                    Cell& adjacentCell = getCell(checkRow, checkCol);

                    // ���� ������ ������, �������� �� ��� ������
                    if (adjacentCell.isEmpty()) {
                        adjacentCell.setState(CellState::Miss);
                        qDebug() << "Marked cell as miss:" << checkRow << checkCol;
                    }
                }
            }
        }
    }
}