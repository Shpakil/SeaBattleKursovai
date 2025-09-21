#pragma once
#include "Player.h"
#include <utility>
#include <string>

class ComputerPlayer : public Player
{
public:
    ComputerPlayer(const std::string& n = "Computer");

    // Автоматическая расстановка кораблей
    void placeShips() override;

    // Делает случайный ход
    std::pair<int, int> makeMove() override;
};


