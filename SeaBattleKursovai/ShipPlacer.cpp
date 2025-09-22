#include "ShipPlacer.h"

ShipPlacer::ShipPlacer()
{
    reset();
}

void ShipPlacer::reset()
{
    shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    currentShipIndex = 0;
    currentOrientation = ShipOrientation::Horizontal;
}

int ShipPlacer::getCurrentShipSize() const
{
    if (currentShipIndex < shipSizes.size())
        return shipSizes[currentShipIndex];
    return -1;
}

ShipOrientation ShipPlacer::getCurrentOrientation() const
{
    return currentOrientation;
}

QString ShipPlacer::getCurrentShipDescription() const
{
    if (isPlacementComplete()) return "All ships placed!";

    int size = getCurrentShipSize();
    QString orientation = (currentOrientation == ShipOrientation::Horizontal)
        ? "Horizontal" : "Vertical";

    QString shipType;
    switch (size) {
    case 4: shipType = "4-deck"; break;
    case 3: shipType = "3-deck"; break;
    case 2: shipType = "2-deck"; break;
    case 1: shipType = "1-deck"; break;
    default: shipType = "unknown";
    }

    return QString("Place %1 ship (%2 orientation)").arg(shipType).arg(orientation);
}

void ShipPlacer::nextShip()
{
    if (currentShipIndex < shipSizes.size())
        currentShipIndex++;
}

void ShipPlacer::toggleOrientation()
{
    currentOrientation = (currentOrientation == ShipOrientation::Horizontal)
        ? ShipOrientation::Vertical
        : ShipOrientation::Horizontal;
}

bool ShipPlacer::isPlacementComplete() const
{
    return currentShipIndex >= shipSizes.size();
}
