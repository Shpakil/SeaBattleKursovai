#include "GameLogic.h"
#include <iostream>

// �����������
GameLogic::GameLogic(Player* p1, Player* p2)
    : player1(p1), player2(p2), current(p1), opponent(p2) {
}

// ������ ����� ����
void GameLogic::startGame()
{
    std::cout << "=== ����� ���� ===" << std::endl;

    // ����� ������ ����� ����������� �������
    player1->placeShips();
    player2->placeShips();

    std::cout << "������� �����������, �������� ���!" << std::endl;
}

// ������� ���
void GameLogic::makeTurn()
{
    // 1. ������� ����� �������� ����������
    std::pair<int, int> move = current->makeMove();
    int row = move.first;
    int col = move.second;

    // 2. ��������� ������� �� ���� ���������
    bool hit = opponent->getBoard().shootAt(row, col);

    // 3. �������� ���������
    if (hit)
    {
        std::cout << current->getName() << " ����� � (" << row << ", " << col << ")!" << std::endl;
    }
    else
    {
        std::cout << current->getName() << " ����������� � (" << row << ", " << col << ")." << std::endl;
    }

    // 4. ���������, �� ����������� �� ����
    if (opponent->getBoard().allShipsSunk())
    {
        std::cout << ">>> " << current->getName() << " �������!" << std::endl;
        return;
    }

    // 5. ���� ����������� � ������ ������
    if (!hit)
        std::swap(current, opponent);
}

// �������� ����� ����
bool GameLogic::isGameOver() const
{
    return player1->getBoard().allShipsSunk() || player2->getBoard().allShipsSunk();
}

// �������� ����������
Player* GameLogic::getWinner() const
{
    if (player1->getBoard().allShipsSunk())
        return player2;
    if (player2->getBoard().allShipsSunk())
        return player1;
    return nullptr;
}

