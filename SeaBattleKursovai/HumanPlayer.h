#pragma once
#include "Player.h"
#include "ShipPlacer.h"
#include <utility>
#include <string>
#include <QString>

class HumanPlayer : public Player
{
private:
    ShipPlacer shipPlacer;
    std::pair<int, int> nextMove;
    bool moveReady;

public:
    HumanPlayer(const std::string& n = "Player");

    void reset() override;
    void placeShips() override;

    bool placeShip(int row, int col);
    void toggleOrientation();
    bool isPlacementComplete() const;
    QString getCurrentShipDescription() const;

    std::pair<int, int> makeMove() override;
    void setNextMove(int row, int col);
};




