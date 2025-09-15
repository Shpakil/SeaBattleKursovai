#include "cell.h"
#include "ship.h"

Cell::Cell()
    : m_state(Empty)
    , m_ship(nullptr)
{
}

Cell::State Cell::state() const
{
    return m_state;
}

void Cell::setState(State state)
{
    m_state = state;
}

bool Cell::hasShip() const
{
    return m_state == Ship || m_state == Hit || m_state == Destroyed;
}

Ship* Cell::ship() const
{
    return m_ship;
}

void Cell::setShip(Ship* ship)
{
    m_ship = ship;
    if (ship) {
        m_state = Ship;
    }
    else {
        m_state = Empty;
    }
}
