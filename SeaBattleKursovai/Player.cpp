#include "Player.h"

Player::Player(const std::string& playerName)
    : name(playerName), board(10, 10)
{
}

void Player::reset()
{
    board = Board(10, 10);
}

Board& Player::getBoard()
{
    return board;
}

const Board& Player::getBoard() const
{
    return board;
}

const std::string& Player::getName() const
{
    return name;
}

bool Player::hasLost() const
{
    return board.allShipsSunk();
}

// Ќовый метод: проверка что корабль можно разместить без касани€ других кораблей
bool Player::isValidShipPlacement(int row, int col, int size, ShipOrientation orientation) const
{
    // ѕроверка выхода за границы
    if (orientation == ShipOrientation::Horizontal) {
        if (col + size > 10) return false;
    }
    else {
        if (row + size > 10) return false;
    }

    // ѕроверка каждой клетки корабл€ и области вокруг
    for (int i = 0; i < size; ++i) {
        int r = row + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = col + (orientation == ShipOrientation::Horizontal ? i : 0);

        if (r >= 10 || c >= 10) return false;

        // ѕроверка самой клетки
        const Cell& cell = getBoard().getCell(r, c);
        if (!cell.isEmpty()) return false;

        // ѕроверка всех соседних клеток (включа€ диагональные)
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = r + dr;
                int nc = c + dc;

                // ѕровер€ем что клетка в пределах доски
                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    const Cell& adjacentCell = getBoard().getCell(nr, nc);
                    if (adjacentCell.hasShip()) {
                        return false;  // –€дом уже есть корабль
                    }
                }
            }
        }
    }

    return true;
}

