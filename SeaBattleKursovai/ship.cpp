#include "ship.h"
#include "cell.h"

Ship::Ship(int size, Direction direction)
    : m_size(size)
    , m_hits(0)
    , m_direction(direction)
{
}

Ship::~Ship()
{
}

int Ship::size() const
{
    return m_size;
}

Ship::Direction Ship::direction() const
{
    return m_direction;
}

bool Ship::isDestroyed() const
{
    return m_hits >= m_size;
}

bool Ship::isPlaced() const
{
    return !m_cells.isEmpty();
}

void Ship::hit()
{
    if (m_hits < m_size) {
        m_hits++;
    }
}

void Ship::addCell(Cell* cell)
{
    if (cell && !m_cells.contains(cell)) {
        m_cells.append(cell);
        cell->setShip(this);
    }
}

const QVector<Cell*>& Ship::cells() const
{
    return m_cells;
}