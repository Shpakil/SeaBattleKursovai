#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& n)
    : Player(n)
{
}

void HumanPlayer::placeShips()
{
    std::cout << "����� " << name << ", ���������� ���� �������." << std::endl;
    // � ������� � ���������� ����� UI
}

std::pair<int, int> HumanPlayer::makeMove()
{
    int row, col;
    std::cout << "��� ������ " << name << ". ������� ���������� (row col): ";
    std::cin >> row >> col;
    return { row, col };
}