#pragma once
#ifndef CELL_H
#define CELL_H

class Ship;

class Cell
{
public:
    enum State {
        Empty,      // ������ ������
        Ship,       // ������ � ��������
        Hit,        // ���������
        Miss,       // ������
        Destroyed   // ������������ �������
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

