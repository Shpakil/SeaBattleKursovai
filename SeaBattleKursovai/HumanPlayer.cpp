#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& n)
    : Player(n)
{
}

void HumanPlayer::placeShips()
{
    std::cout << "Игрок " << name << ", разместите свои корабли." << std::endl;
    // В будущем — реализация через UI
}

std::pair<int, int> HumanPlayer::makeMove()
{
    int row, col;
    std::cout << "Ход игрока " << name << ". Введите координаты (row col): ";
    std::cin >> row >> col;
    return { row, col };
}