#pragma once
#include "Player.h"
#include <utility>  // ��� std::pair
#include <string>

class HumanPlayer : public Player
{
public:
    HumanPlayer(const std::string& n);

    // ����������� ������� ������� (����� ����� UI)
    void placeShips() override;

    // ������ ��� (���������� ����� ������������ �� UI)
    std::pair<int, int> makeMove() override;
};


