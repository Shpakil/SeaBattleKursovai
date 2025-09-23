#pragma once
#include "Player.h"
#include "ShipPlacer.h"

class HumanPlayer : public Player
{
private:
    ShipPlacer shipPlacer;
    bool moveReady;
    std::pair<int, int> nextMove;

public:
    HumanPlayer(const std::string& n = "Human");
    void reset() override;
    void placeShips() override;
    bool placeShip(int row, int col);
    void toggleOrientation();
    bool isPlacementComplete() const;
    QString getCurrentShipDescription() const;
    std::pair<int, int> makeMove() override;
    void setNextMove(int row, int col);
};



