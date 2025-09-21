#include "ComputerPlayer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

ComputerPlayer::ComputerPlayer(const std::string& n)
    : Player(n)
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // инициализация генератора случайных чисел
}

void ComputerPlayer::placeShips()
{
    std::cout << "Компьютер " << name << " размещает корабли автоматически..." << std::endl;
  
}

std::pair<int, int> ComputerPlayer::makeMove()
{
    int row = std::rand() % 10;
    int col = std::rand() % 10;
    std::cout << "Компьютер " << name << " стреляет в (" << row << ", " << col << ")" << std::endl;
    return { row, col };
}
