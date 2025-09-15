#include "board.h"
#include <QDebug>

Board::Board()
{
    // Инициализация сетки
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            m_grid[row][col] = Cell();
        }
    }
}

Board::~Board()
{
    qDeleteAll(m_ships);
    m_ships.clear();
}

Cell* Board::cellAt(int row, int col)
{
    if (isWithinBounds(row, col)) {
        return &m_grid[row][col];
    }
    return nullptr;
}

const Cell* Board::cellAt(int row, int col) const
{
    if (isWithinBounds(row, col)) {
        return &m_grid[row][col];
    }
    return nullptr;
}

bool Board::placeShip(int row, int col, Ship* ship)
{
    if (!ship || !isValidPlacement(row, col, ship)) {
        return false;
    }

    int dr = (ship->direction() == Ship::Vertical) ? 1 : 0;
    int dc = (ship->direction() == Ship::Horizontal) ? 1 : 0;

    for (int i = 0; i < ship->size(); ++i) {
        int r = row + i * dr;
        int c = col + i * dc;

        if (isWithinBounds(r, c)) {
            ship->addCell(&m_grid[r][c]);
        }
    }

    m_ships.append(ship);
    return true;
}

bool Board::isValidPlacement(int row, int col, const Ship* ship) const
{
    if (!ship || !isWithinBounds(row, col)) {
        return false;
    }

    int dr = (ship->direction() == Ship::Vertical) ? 1 : 0;
    int dc = (ship->direction() == Ship::Horizontal) ? 1 : 0;

    // Проверка, что корабль помещается на доску
    int endRow = row + (ship->size() - 1) * dr;
    int endCol = col + (ship->size() - 1) * dc;

    if (!isWithinBounds(endRow, endCol)) {
        return false;
    }

    // Проверка на пересечение с другими кораблями и соседние клетки
    for (int i = 0; i < ship->size(); ++i) {
        int r = row + i * dr;
        int c = col + i * dc;

        if (m_grid[r][c].hasShip()) {
            return false;
        }

        if (hasAdjacentShips(r, c, ship)) {
            return false;
        }
    }

    return true;
}

bool Board::allShipsDestroyed() const
{
    for (const Ship* ship : m_ships) {
        if (!ship->isDestroyed()) {
            return false;
        }
    }
    return true;
}

bool Board::attack(int row, int col)
{
    if (!isWithinBounds(row, col)) {
        return false;
    }

    Cell& cell = m_grid[row][col];

    if (cell.state() == Cell::Hit || cell.state() == Cell::Miss) {
        return false; // Уже стреляли сюда
    }

    if (cell.hasShip()) {
        cell.setState(Cell::Hit);
        if (cell.ship()) {
            cell.ship()->hit();
            if (cell.ship()->isDestroyed()) {
                // Помечаем все клетки корабля как уничтоженные
                for (Cell* shipCell : cell.ship()->cells()) {
                    shipCell->setState(Cell::Destroyed);
                }
            }
        }
        return true;
    }
    else {
        cell.setState(Cell::Miss);
        return false;
    }
}

bool Board::isValidAttack(int row, int col) const
{
    if (!isWithinBounds(row, col)) {
        return false;
    }

    const Cell& cell = m_grid[row][col];
    return cell.state() != Cell::Hit && cell.state() != Cell::Miss;
}

const QVector<Ship*>& Board::ships() const
{
    return m_ships;
}

bool Board::isWithinBounds(int row, int col) const
{
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

bool Board::hasAdjacentShips(int row, int col, const Ship* currentShip) const
{
    // Проверяем все соседние клетки (включая диагональные)
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int r = row + dr;
            int c = col + dc;

            if (isWithinBounds(r, c) && m_grid[r][c].hasShip()) {
                // Если это не клетка текущего корабля
                if (m_grid[r][c].ship() != currentShip) {
                    return true;
                }
            }
        }
    }
    return false;
}
