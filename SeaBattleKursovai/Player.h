#pragma once
#include <string>
#include <utility>   // для std::pair
#include "Board.h"  

class Player
{
protected:

    std::string name;   // имя игрока
    Board board;    // игровое поле игрока

public:
    Player(const std::string& playerName);

    virtual ~Player() = default;

    // Чисто виртуальные методы
    virtual void placeShips() = 0;                     // расстановка кораблей
    virtual std::pair<int, int> makeMove() = 0;        // выбор клетки для выстрела

    // Общие методы
    Board& getBoard();                             // получить ссылку на поле
    const std::string& getName() const;               // имя игрока
    bool hasLost() const;                              // проверка потопления всех кораблей
};


