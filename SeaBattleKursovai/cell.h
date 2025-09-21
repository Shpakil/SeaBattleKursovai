#pragma once
#include <iostream>

// ��������� ������
enum class CellState
{
    Empty,
    Ship,
    Hit,
    Miss
};

class Cell
{
private:
    int row;
    int col;
    CellState state;

public:
    // �����������
    Cell(int r = 0, int c = 0);

    // �������
    int getRow() const;
    int getCol() const;
    CellState getState() const;

    // ������
    void setState(CellState s);

    // ��������
    bool isEmpty() const;
    bool hasShip() const;
    bool isHit() const;
    bool isMiss() const;

    // "������� �� ������"
    bool shoot();
};


