#pragma once
#include "Board.h"
#include "Player.h"

class GameLogic
{
private:
    Player* player1;   // ����� 1
    Player* player2;   // ����� 2
    Player* current;   // ������� �����
    Player* opponent;  // ���������

public:
    GameLogic(Player* p1, Player* p2);

    void startGame();        // ������������� ����
    // GameLogic.cpp
    bool makeTurn(int row, int col);
    bool isGameOver() const; // �������� ���������
    Player* getWinner() const; // ��� �������
};


