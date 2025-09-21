#pragma once
#include "Player.h"
#include <utility>  // для std::pair
#include <string>

class HumanPlayer : public Player
{
public:
    HumanPlayer(const std::string& n);

    // Расставляет корабли вручную (позже будет UI)
    void placeShips() override;

    // Делает ход (координаты будут передаваться из UI)
    std::pair<int, int> makeMove() override;
};


