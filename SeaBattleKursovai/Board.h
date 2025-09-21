#pragma once
#include <vector>
#include "Cell.h"
#include "Ship.h"

class Board
{
private:
    int rows, cols;
    std::vector<std::vector<Cell>> grid;  // сетка клеток
    std::vector<Ship> ships;              // корабли на поле

public:
    Board(int r = 10, int c = 10);        // конструктор с размерами

    // Получить клетку по координатам
    Cell& getCell(int row, int col);

    // Расстановка корабля (передаём указатели на клетки)
    bool placeShip(const std::vector<Cell*>& shipCells);

    // Выстрел по клетке
    bool shootAt(int row, int col);

    // Проверка, потоплены ли все корабли
    bool allShipsSunk() const;

    // Получить сетку (для UI)
    const std::vector<std::vector<Cell>>& getGrid() const;
};


