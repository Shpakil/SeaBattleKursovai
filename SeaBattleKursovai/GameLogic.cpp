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

// Сделать ход
void GameLogic::makeTurn()
{
    // 1. Текущий игрок выбирает координаты
    std::pair<int, int> move = current->makeMove();
    int row = move.first;
    int col = move.second;

    // 2. Выполняем выстрел по полю соперника
    bool hit = opponent->getBoard().shootAt(row, col);

    // 3. Сообщаем результат
    if (hit)
    {
        std::cout << current->getName() << " попал в (" << row << ", " << col << ")!" << std::endl;
    }
    else
    {
        std::cout << current->getName() << " промахнулся в (" << row << ", " << col << ")." << std::endl;
    }

    // 4. Проверяем, не закончилась ли игра
    if (opponent->getBoard().allShipsSunk())
    {
        std::cout << ">>> " << current->getName() << " выиграл!" << std::endl;
        return;
    }

    // 5. Если промахнулся — меняем игрока
    if (!hit)
        std::swap(current, opponent);
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

