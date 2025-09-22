#pragma once
#include "Player.h"
#include <utility>
#include <string>
#include <vector>


class ComputerPlayer : public Player
{
private:
    std::vector<std::pair<int, int>> availableMoves;

public:
    ComputerPlayer(const std::string& n = "Computer");

    void reset() override;
    void placeShips() override;
    std::pair<int, int> makeMove() override;

private:
    bool tryPlaceShip(int startRow, int startCol, int size, ShipOrientation orientation);
    bool isValidPlacement(int row, int col, int size, ShipOrientation orientation) const;
    void generateAvailableMoves();
};


