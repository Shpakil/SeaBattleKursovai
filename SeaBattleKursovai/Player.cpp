#include "Player.h"

Player::Player(const std::string& playerName)
    : name(playerName)
{
    // ����������� �������������� ��� � ������� ���� �� ���������
}

Board& Player::getBoard()
{
    return board;
}

const std::string& Player::getName() const
{
    return name;
}

bool Player::hasLost() const
{
    // ���������, �������� �� ����� �������
    return board.allShipsSunk();  // ��������������, ��� GameBoard ����� ��������� ���
}

