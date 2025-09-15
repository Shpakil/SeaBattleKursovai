#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <QVector>

class Cell;

class Ship
{
public:
    enum Direction {
        Horizontal,
        Vertical
    };

    Ship(int size, Direction direction);
    ~Ship();

    int size() const;
    Direction direction() const;
    bool isDestroyed() const;
    bool isPlaced() const;

    void hit();
    void addCell(Cell* cell);
    const QVector<Cell*>& cells() const;

private:
    int m_size;
    int m_hits;
    Direction m_direction;
    QVector<Cell*> m_cells;
};

#endif // SHIP_H

