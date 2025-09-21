#pragma once
#include "Board.h"
#include "Player.h"

class GameLogic
{
private:
    Player* player1;   // Игрок 1
    Player* player2;   // Игрок 2
    Player* current;   // Текущий игрок
    Player* opponent;  // Противник

public:
    GameLogic(Player* p1, Player* p2);

    void startGame();        // Инициализация игры
    void makeTurn();         // Сделать один ход
    bool isGameOver() const; // Проверка окончания
    Player* getWinner() const; // Кто победил
};


