#include "ComputerPlayer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

ComputerPlayer::ComputerPlayer(const std::string& n)
    : Player(n)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    reset();
}

void ComputerPlayer::reset()
{
    Player::reset();
    generateAvailableMoves();
}

void ComputerPlayer::generateAvailableMoves()
{
    availableMoves.clear();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            availableMoves.push_back({ i, j });
        }
    }
}

void ComputerPlayer::placeShips()
{
    std::vector<int> shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int size : shipSizes) {
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 1000) {
            int row = rand() % 10;
            int col = rand() % 10;
            ShipOrientation orientation = (rand() % 2 == 0) ?
                ShipOrientation::Horizontal : ShipOrientation::Vertical;

            if (isValidPlacement(row, col, size, orientation)) {
                placed = tryPlaceShip(row, col, size, orientation);
            }
            attempts++;
        }

        if (!placed) {
            std::cout << "Failed to place ship of size " << size << std::endl;
        }
    }

    std::cout << "Computer finished placing ships" << std::endl;
}

bool ComputerPlayer::isValidPlacement(int row, int col, int size, ShipOrientation orientation) const
{
    // Проверяем границы доски
    if (orientation == ShipOrientation::Horizontal) {
        if (col + size > 10) return false;
    }
    else {
        if (row + size > 10) return false;
    }

    // Проверяем клетки и окружающее пространство
    for (int i = -1; i <= size; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int r = row + (orientation == ShipOrientation::Vertical ? i : j);
            int c = col + (orientation == ShipOrientation::Horizontal ? i : j);

            if (r >= 0 && r < 10 && c >= 0 && c < 10) {
                const Cell& cell = getBoard().getCell(r, c);
                if (!cell.isEmpty()) return false;
            }
        }
    }

    return true;
}

bool ComputerPlayer::tryPlaceShip(int startRow, int startCol, int size, ShipOrientation orientation)
{
    std::vector<Cell*> shipCells;

    // Собираем клетки для корабля
    for (int i = 0; i < size; ++i) {
        int r = startRow + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = startCol + (orientation == ShipOrientation::Horizontal ? i : 0);

        if (r >= 10 || c >= 10) return false;

        Cell& cell = getBoard().getCell(r, c);
        shipCells.push_back(&cell);
    }

    // Пытаемся разместить корабль
    return getBoard().placeShip(shipCells);
}

std::pair<int, int> ComputerPlayer::makeMove()
{
    if (availableMoves.empty()) {
        generateAvailableMoves(); // Регенерируем ходы, если закончились
    }

    int index = rand() % availableMoves.size();
    auto move = availableMoves[index];
    availableMoves.erase(availableMoves.begin() + index);

    std::cout << "Computer attacks: (" << move.first << ", " << move.second << ")" << std::endl;
    return move;
}
