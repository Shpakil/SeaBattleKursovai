#pragma once
#include <string>
#include <utility>
#include "Board.h"
#include "ShipOrientation.h"

class Player
{
protected:
    std::string name;
    Board board;

public:
    Player(const std::string& playerName);
    virtual ~Player() = default;

    virtual void placeShips() = 0;
    virtual std::pair<int, int> makeMove() = 0;
    virtual void reset();

    Board& getBoard();
    const Board& getBoard() const;
    const std::string& getName() const;
    bool hasLost() const;
};


