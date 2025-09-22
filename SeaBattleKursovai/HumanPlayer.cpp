#include "HumanPlayer.h"
#include <QCoreApplication>
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& n)
    : Player(n), moveReady(false)
{
}

void HumanPlayer::reset()
{
    Player::reset();
    shipPlacer.reset();
    moveReady = false;
}

void HumanPlayer::placeShips()
{
    // Ships are placed interactively through UI clicks
    // This method is called but ships are placed via placeShip()
}

bool HumanPlayer::placeShip(int row, int col)
{
    if (shipPlacer.isPlacementComplete()) return false;

    int shipSize = shipPlacer.getCurrentShipSize();
    ShipOrientation orientation = shipPlacer.getCurrentOrientation();

    // Check if ship can be placed
    for (int i = 0; i < shipSize; ++i) {
        int r = row + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = col + (orientation == ShipOrientation::Horizontal ? i : 0);

        if (r >= 10 || c >= 10) return false;

        Cell& cell = getBoard().getCell(r, c);
        if (!cell.isEmpty()) return false;
    }

    // Place the ship
    std::vector<Cell*> shipCells;
    for (int i = 0; i < shipSize; ++i) {
        int r = row + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = col + (orientation == ShipOrientation::Horizontal ? i : 0);
        shipCells.push_back(&getBoard().getCell(r, c));
    }

    if (getBoard().placeShip(shipCells)) {
        shipPlacer.nextShip();
        return true;
    }

    return false;
}

void HumanPlayer::toggleOrientation()
{
    shipPlacer.toggleOrientation();
}

bool HumanPlayer::isPlacementComplete() const
{
    return shipPlacer.isPlacementComplete();
}

QString HumanPlayer::getCurrentShipDescription() const
{
    return shipPlacer.getCurrentShipDescription();
}

std::pair<int, int> HumanPlayer::makeMove()
{
    moveReady = false;
    while (!moveReady) {
        QCoreApplication::processEvents();
    }
    return nextMove;
}

void HumanPlayer::setNextMove(int row, int col)
{
    nextMove = { row, col };
    moveReady = true;
}
