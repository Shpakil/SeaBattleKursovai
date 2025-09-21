#include "Cell.h"

// �����������
Cell::Cell(int r, int c)
    : row(r), col(c), state(CellState::Empty)
{
}

// �������
int Cell::getRow() const
{
    return row;
}

int Cell::getCol() const
{
    return col;
}

CellState Cell::getState() const
{
    return state;
}

// ������
void Cell::setState(CellState s)
{
    state = s;
}

// ��������
bool Cell::isEmpty() const
{
    return state == CellState::Empty;
}

bool Cell::hasShip() const
{
    return state == CellState::Ship;
}

bool Cell::isHit() const
{
    return state == CellState::Hit;
}

bool Cell::isMiss() const
{
    return state == CellState::Miss;
}

// "������� �� ������"
bool Cell::shoot()
{
    if (state == CellState::Ship)
    {
        state = CellState::Hit;
        return true; // ���������
    }
    if (state == CellState::Empty)
    {
        state = CellState::Miss;
        return false; // ������
    }
    return false; // ��������� �������
}