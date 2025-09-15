#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include "cell.h"
#include "ship.h"

class Board
{
public:
    static const int SIZE = 10;

    Board();
    ~Board();

    Cell* cellAt(int row, int col);
    const Cell* cellAt(int row, int col) const;

    bool placeShip(int row, int col, Ship* ship);
    bool isValidPlacement(int row, int col, const Ship* ship) const;
    bool allShipsDestroyed() const;

    bool attack(int row, int col);
    bool isValidAttack(int row, int col) const;

    const QVector<Ship*>& ships() const;

private:
    bool isWithinBounds(int row, int col) const;
    bool hasAdjacentShips(int row, int col, const Ship* ship) const;

    Cell m_grid[SIZE][SIZE];
    QVector<Ship*> m_ships;
};

#endif // BOARD_H

