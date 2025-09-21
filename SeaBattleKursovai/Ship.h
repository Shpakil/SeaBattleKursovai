#pragma once
#include <vector>
#include "Cell.h"

class Ship
{
private:
    std::vector<Cell*> cells;  // клетки, которые занимает корабль

public:
    // Конструктор
    Ship(const std::vector<Cell*>& shipCells);

    // Проверка, потоплен ли корабль
    bool isSunk() const;

    // Получить клетки корабля 
    const std::vector<Cell*>& getCells() const;
};

