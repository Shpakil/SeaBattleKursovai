#pragma once
#include <string>
#include <utility>   // ��� std::pair
#include "Board.h"  

class Player
{
protected:

    std::string name;   // ��� ������
    Board board;    // ������� ���� ������

public:
    Player(const std::string& playerName);

    virtual ~Player() = default;

    // ����� ����������� ������
    virtual void placeShips() = 0;                     // ����������� ��������
    virtual std::pair<int, int> makeMove() = 0;        // ����� ������ ��� ��������

    // ����� ������
    Board& getBoard();                             // �������� ������ �� ����
    const std::string& getName() const;               // ��� ������
    bool hasLost() const;                              // �������� ���������� ���� ��������
};


