#pragma once
#include <vector>
#include <QString>
#include "ShipOrientation.h"

class ShipPlacer
{
private:
    std::vector<int> shipSizes;
    size_t currentShipIndex;
    ShipOrientation currentOrientation;

public:
    ShipPlacer();

    void reset();
    int getCurrentShipSize() const;
    ShipOrientation getCurrentOrientation() const;
    QString getCurrentShipDescription() const;
    void nextShip();
    void toggleOrientation();
    bool isPlacementComplete() const;
};

