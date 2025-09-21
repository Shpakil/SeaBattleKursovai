#include "Cell.h"

// Конструктор
Cell::Cell(int r, int c)
    : row(r), col(c), state(CellState::Empty)
{
}

// Геттеры
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

// Сеттер
void Cell::setState(CellState s)
{
    state = s;
}

// Проверки
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

// "Выстрел по клетке"
bool Cell::shoot()
{
    if (state == CellState::Ship)
    {
        state = CellState::Hit;
        return true; // попадание
    }
    if (state == CellState::Empty)
    {
        state = CellState::Miss;
        return false; // промах
    }
    return false; // повторный выстрел
}