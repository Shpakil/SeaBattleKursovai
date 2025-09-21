#include "ComputerPlayer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

ComputerPlayer::ComputerPlayer(const std::string& n)
    : Player(n)
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // ������������� ���������� ��������� �����
}

void ComputerPlayer::placeShips()
{
    std::cout << "��������� " << name << " ��������� ������� �������������..." << std::endl;
  
}

std::pair<int, int> ComputerPlayer::makeMove()
{
    int row = std::rand() % 10;
    int col = std::rand() % 10;
    std::cout << "��������� " << name << " �������� � (" << row << ", " << col << ")" << std::endl;
    return { row, col };
}
