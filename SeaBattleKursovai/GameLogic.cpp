#include "GameLogic.h"
#include <iostream>

// Конструктор
GameLogic::GameLogic(Player* p1, Player* p2)
    : player1(p1), player2(p2), current(p1), opponent(p2) {
}

// Запуск новой игры
void GameLogic::startGame()
{
    std::cout << "=== Новая игра ===" << std::endl;

    // Здесь каждый игрок расставляет корабли
    player1->placeShips();
    player2->placeShips();

    std::cout << "Корабли расставлены, начинаем бой!" << std::endl;
}

// GameLogic.cpp
bool GameLogic::makeTurn(int row, int col)
{
    if (isGameOver())
        return false;

    bool hit = opponent->getBoard().shootAt(row, col);

    if (!hit)
        std::swap(current, opponent);

    return hit;
}

// Проверка конца игры
bool GameLogic::isGameOver() const
{
    return player1->getBoard().allShipsSunk() || player2->getBoard().allShipsSunk();
}

// Получить победителя
Player* GameLogic::getWinner() const
{
    if (player1->getBoard().allShipsSunk())
        return player2;
    if (player2->getBoard().allShipsSunk())
        return player1;
    return nullptr;
}

