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

bool ComputerPlayer::isValidPlacement(int row, int col, int size, ShipOrientation orientation) const
{
    // Проверяем границы доски
    if (orientation == ShipOrientation::Horizontal) {
        if (col + size > 10) return false;
    }
    else {
        if (row + size > 10) return false;
    }

    // Проверяем каждую клетку корабля и область вокруг
    for (int i = 0; i < size; ++i) {
        int r = row + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = col + (orientation == ShipOrientation::Horizontal ? i : 0);

        if (r >= 10 || c >= 10) return false;

        const Cell& cell = getBoard().getCell(r, c);

        // Клетка должна быть пустой
        if (!cell.isEmpty()) return false;

        // Вокруг клетки не должно быть кораблей
        if (!getBoard().isAreaClear(r, c)) return false;
    }

    return true;
}

bool ComputerPlayer::tryPlaceShip(int startRow, int startCol, int size, ShipOrientation orientation)
{
    std::vector<Cell*> shipCells;

    for (int i = 0; i < size; ++i) {
        int r = startRow + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = startCol + (orientation == ShipOrientation::Horizontal ? i : 0);

        if (r >= 10 || c >= 10) return false;

        Cell& cell = getBoard().getCell(r, c);
        shipCells.push_back(&cell);
    }

    return getBoard().placeShip(shipCells);
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

std::pair<int, int> ComputerPlayer::makeMove()
{
    if (availableMoves.empty()) {
        generateAvailableMoves();
    }

    int index = rand() % availableMoves.size();
    auto move = availableMoves[index];
    availableMoves.erase(availableMoves.begin() + index);

    return move;
}
