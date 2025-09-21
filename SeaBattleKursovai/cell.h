#pragma once
#include <iostream>

// Состояние клетки
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
    // Конструктор
    Cell(int r = 0, int c = 0);

    // Геттеры
    int getRow() const;
    int getCol() const;
    CellState getState() const;

    // Сеттер
    void setState(CellState s);

    // Проверки
    bool isEmpty() const;
    bool hasShip() const;
    bool isHit() const;
    bool isMiss() const;

    // "Выстрел по клетке"
    bool shoot();
};


