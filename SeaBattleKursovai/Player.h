#pragma once
#include <string>
#include "Board.h"
#include "ShipOrientation.h"
class Player
{
private:
    std::string name;
    Board board;

public:
    Player(const std::string& playerName = "Player");
    virtual ~Player() = default;

    virtual void reset();
    virtual void placeShips() = 0;
    virtual std::pair<int, int> makeMove() = 0;

    Board& getBoard();
    const Board& getBoard() const;
    const std::string& getName() const;
    bool hasLost() const;

    // Новый метод для проверки валидности размещения корабля
    bool isValidShipPlacement(int row, int col, int size, ShipOrientation orientation) const;
};


