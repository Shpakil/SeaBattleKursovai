#pragma once
#include <vector>
#include "Cell.h"
#include "Ship.h"

class Board
{
private:
    int rows, cols;
    std::vector<std::vector<Cell>> grid;  // ����� ������
    std::vector<Ship> ships;              // ������� �� ����

public:
    Board(int r = 10, int c = 10);        // ����������� � ���������

    // �������� ������ �� �����������
    Cell& getCell(int row, int col);

    // ����������� ������� (������� ��������� �� ������)
    bool placeShip(const std::vector<Cell*>& shipCells);

    // ������� �� ������
    bool shootAt(int row, int col);

    // ��������, ��������� �� ��� �������
    bool allShipsSunk() const;

    // �������� ����� (��� UI)
    const std::vector<std::vector<Cell>>& getGrid() const;
};


