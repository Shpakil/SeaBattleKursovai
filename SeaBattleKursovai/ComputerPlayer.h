#pragma once
#include "Player.h"
#include <utility>
#include <string>

class ComputerPlayer : public Player
{
public:
    ComputerPlayer(const std::string& n = "���������");

    // �������������� ����������� ��������
    void placeShips() override;

    // ������ ��������� ���
    std::pair<int, int> makeMove() override;
};


