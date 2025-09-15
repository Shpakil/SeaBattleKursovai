#pragma once
#ifndef CELL_H
#define CELL_H

class Ship;

class Cell
{
public:
    enum State {
        Empty,      // Пустая клетка
        Ship,       // Клетка с кораблем
        Hit,        // Попадание
        Miss,       // Промах
        Destroyed   // Уничтоженный корабль
    };

    Cell();

    State state() const;
    void setState(State state);

    bool hasShip() const;
    Ship* ship() const;
    void setShip(Ship* ship);

private:
    State m_state;
    Ship* m_ship;
};

#endif // CELL_H

