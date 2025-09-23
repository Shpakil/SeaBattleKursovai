#pragma once
#include "Player.h"
#include <utility>
#include <string>
#include <vector>
#include <queue>

enum class AIMode {
    Search,     // Режим поиска кораблей
    Hunt        // Режим добивания раненого корабля
};

class ComputerPlayer : public Player
{
private:
    std::vector<std::pair<int, int>> availableMoves;

    // Логика добивания
    AIMode currentMode;
    std::pair<int, int> firstHit;          // Первое попадание по кораблю
    std::pair<int, int> lastHit;           // Последнее попадание по кораблю
    std::queue<std::pair<int, int>> huntMoves; // Очередь ходов для добивания
    ShipOrientation suspectedOrientation;  // Предполагаемая ориентация корабля
    bool orientationConfirmed;             // Ориентация подтверждена

public:
    ComputerPlayer(const std::string& n = "Computer");
    void updateAIMode(int lastRow, int lastCol, bool hit, bool sink = false);
    void reset() override;
    void placeShips() override;
    std::pair<int, int> makeMove() override;
    void removeMoveFromAvailable(int row, int col);

    // НОВЫЙ МЕТОД: Синхронизация с доской противника
    void syncAvailableMovesWithBoard(const Board& opponentBoard);

private:
    bool tryPlaceShip(int startRow, int startCol, int size, ShipOrientation orientation);
    void generateAvailableMoves();
    void clearHuntState();

    // Методы для интеллектуальной стрельбы
    std::pair<int, int> makeSearchMove();  // Случайный поиск
    std::pair<int, int> makeHuntMove();    // Добивание корабля

    void generateHuntMoves(int hitRow, int hitCol);
    void generateTargetedHuntMoves();      // Целевое добивание после определения ориентации
    bool isValidHuntTarget(int row, int col) const;
};


