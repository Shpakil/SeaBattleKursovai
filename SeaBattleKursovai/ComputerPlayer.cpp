#include "ComputerPlayer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <QDebug>

ComputerPlayer::ComputerPlayer(const std::string& n)
    : Player(n), currentMode(AIMode::Search),
    suspectedOrientation(ShipOrientation::Horizontal),
    orientationConfirmed(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    reset();
}

void ComputerPlayer::reset()
{
    Player::reset();
    generateAvailableMoves();
    currentMode = AIMode::Search;
    clearHuntState();
}

void ComputerPlayer::clearHuntState()
{
    firstHit = { -1, -1 };
    lastHit = { -1, -1 };
    while (!huntMoves.empty()) huntMoves.pop();
    suspectedOrientation = ShipOrientation::Horizontal;
    orientationConfirmed = false;
}

void ComputerPlayer::generateAvailableMoves()
{
    availableMoves.clear();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            availableMoves.push_back({ i, j });
        }
    }
}

void ComputerPlayer::removeMoveFromAvailable(int row, int col)
{
    auto it = std::find(availableMoves.begin(), availableMoves.end(),
        std::make_pair(row, col));
    if (it != availableMoves.end()) {
        availableMoves.erase(it);
        qDebug() << "Removed move from available: (" << row << ", " << col << ")";
    }
}

// НОВЫЙ МЕТОД: Синхронизация доступных ходов с состоянием доски
void ComputerPlayer::syncAvailableMovesWithBoard(const Board& opponentBoard)
{
    // Создаем временный список валидных ходов
    std::vector<std::pair<int, int>> validMoves;

    for (const auto& move : availableMoves) {
        int row = move.first;
        int col = move.second;

        // Проверяем, что по этой клетке еще не стреляли
        const Cell& cell = opponentBoard.getCell(row, col);
        if (!cell.isHit() && !cell.isMiss()) {
            validMoves.push_back(move);
        }
        else {
            qDebug() << "Syncing: Removing already shot cell (" << row << ", " << col << ")";
        }
    }

    availableMoves = validMoves;
    qDebug() << "Available moves after sync:" << availableMoves.size();
}

bool ComputerPlayer::tryPlaceShip(int startRow, int startCol, int size, ShipOrientation orientation)
{
    if (!isValidShipPlacement(startRow, startCol, size, orientation)) {
        return false;
    }

    std::vector<Cell*> shipCells;
    for (int i = 0; i < size; ++i) {
        int r = startRow + (orientation == ShipOrientation::Vertical ? i : 0);
        int c = startCol + (orientation == ShipOrientation::Horizontal ? i : 0);

        if (r >= 10 || c >= 10) return false;

        Cell& cell = getBoard().getCell(r, c);
        shipCells.push_back(&cell);
    }

    return getBoard().placeShip(shipCells);
}

void ComputerPlayer::placeShips()
{
    std::vector<int> shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int size : shipSizes) {
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 1000) {
            int row = rand() % 10;
            int col = rand() % 10;
            ShipOrientation orientation = (rand() % 2 == 0) ?
                ShipOrientation::Horizontal : ShipOrientation::Vertical;

            if (tryPlaceShip(row, col, size, orientation)) {
                placed = true;
            }
            attempts++;
        }

        if (!placed) {
            std::cout << "Failed to place ship of size " << size << std::endl;
        }
    }

    std::cout << "Computer finished placing ships" << std::endl;
}

std::pair<int, int> ComputerPlayer::makeMove()
{
    qDebug() << "Computer AI mode:" << (currentMode == AIMode::Search ? "SEARCH" : "HUNT");
    qDebug() << "Available moves:" << availableMoves.size();
    qDebug() << "Hunt moves in queue:" << huntMoves.size();

    std::pair<int, int> move;

    if (currentMode == AIMode::Hunt && !huntMoves.empty()) {
        move = makeHuntMove();
    }
    else {
        move = makeSearchMove();
    }

    // Удаляем ход из доступных
    removeMoveFromAvailable(move.first, move.second);

    qDebug() << "Computer attacks: (" << move.first << ", " << move.second << ")";
    return move;
}

std::pair<int, int> ComputerPlayer::makeSearchMove()
{
    if (availableMoves.empty()) {
        qDebug() << "No available moves in search mode!";
        return { -1, -1 };
    }

    int index = rand() % availableMoves.size();
    auto move = availableMoves[index];
    return move;
}

std::pair<int, int> ComputerPlayer::makeHuntMove()
{
    qDebug() << "Making hunt move. Queue size:" << huntMoves.size();

    // Очищаем очередь от невалидных ходов
    while (!huntMoves.empty()) {
        auto frontMove = huntMoves.front();
        if (isValidHuntTarget(frontMove.first, frontMove.second)) {
            auto move = frontMove;
            huntMoves.pop();
            return move;
        }
        else {
            qDebug() << "Removing invalid hunt move:" << frontMove.first << frontMove.second;
            huntMoves.pop();
        }
    }

    // Если очередь пуста, но мы в режиме охоты, генерируем новые ходы
    if (currentMode == AIMode::Hunt) {
        qDebug() << "Hunt queue empty, generating new moves around last hit: ("
            << lastHit.first << ", " << lastHit.second << ")";

        if (orientationConfirmed) {
            generateTargetedHuntMoves();
        }
        else {
            generateHuntMoves(lastHit.first, lastHit.second);
        }

        // Пытаемся снова получить ход
        if (!huntMoves.empty()) {
            return makeHuntMove();
        }

        // Если не удалось сгенерировать ходы, но корабль еще не добит,
        // используем поисковый ход, но остаемся в режиме охоты
        qDebug() << "No hunt moves generated, using search move but staying in HUNT mode";
    }

    return makeSearchMove();
}

bool ComputerPlayer::isValidHuntTarget(int row, int col) const
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        return false;
    }

    return std::find(availableMoves.begin(), availableMoves.end(),
        std::make_pair(row, col)) != availableMoves.end();
}

void ComputerPlayer::generateHuntMoves(int hitRow, int hitCol)
{
    qDebug() << "Generating hunt moves around hit at:" << hitRow << hitCol;

    std::vector<std::pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    int validMovesAdded = 0;

    for (const auto& dir : directions) {
        int newRow = hitRow + dir.first;
        int newCol = hitCol + dir.second;

        if (isValidHuntTarget(newRow, newCol)) {
            huntMoves.push({ newRow, newCol });
            validMovesAdded++;
            qDebug() << "Added hunt move:" << newRow << newCol;
        }
    }

    qDebug() << "Generated" << validMovesAdded << "valid hunt moves";
}

void ComputerPlayer::generateTargetedHuntMoves()
{
    qDebug() << "Generating targeted hunt moves with confirmed orientation: "
        << (suspectedOrientation == ShipOrientation::Horizontal ? "Horizontal" : "Vertical");

    if (lastHit.first == -1 || lastHit.second == -1) {
        qDebug() << "No last hit coordinates for targeted hunt";
        return;
    }

    int validMovesAdded = 0;

    if (suspectedOrientation == ShipOrientation::Horizontal) {
        // Горизонтальный корабль - проверяем клетки слева и справа
        if (isValidHuntTarget(lastHit.first, lastHit.second - 1)) {
            huntMoves.push({ lastHit.first, lastHit.second - 1 });
            validMovesAdded++;
        }
        if (isValidHuntTarget(lastHit.first, lastHit.second + 1)) {
            huntMoves.push({ lastHit.first, lastHit.second + 1 });
            validMovesAdded++;
        }
    }
    else {
        // Вертикальный корабль - проверяем клетки сверху и снизу
        if (isValidHuntTarget(lastHit.first - 1, lastHit.second)) {
            huntMoves.push({ lastHit.first - 1, lastHit.second });
            validMovesAdded++;
        }
        if (isValidHuntTarget(lastHit.first + 1, lastHit.second)) {
            huntMoves.push({ lastHit.first + 1, lastHit.second });
            validMovesAdded++;
        }
    }

    qDebug() << "Generated" << validMovesAdded << "valid targeted hunt moves";
}

void ComputerPlayer::updateAIMode(int lastRow, int lastCol, bool hit, bool sink)
{
    if (sink) {
        qDebug() << "Ship sunk! Returning to search mode.";
        clearHuntState();
        currentMode = AIMode::Search;
        return;
    }

    if (hit) {
        qDebug() << "Computer HIT at:" << lastRow << lastCol;

        if (currentMode == AIMode::Search) {
            // Первое попадание - переключаемся в режим охоты
            currentMode = AIMode::Hunt;
            firstHit = { lastRow, lastCol };
            lastHit = { lastRow, lastCol };
            orientationConfirmed = false;
            generateHuntMoves(lastRow, lastCol);
        }
        else {
            // Уже в режиме охоты - обновляем информацию
            lastHit = { lastRow, lastCol };

            // Определяем ориентацию корабля
            if (!orientationConfirmed) {
                if (firstHit.first == lastHit.first) {
                    suspectedOrientation = ShipOrientation::Horizontal;
                    orientationConfirmed = true;
                    qDebug() << "Orientation confirmed: Horizontal";
                }
                else if (firstHit.second == lastHit.second) {
                    suspectedOrientation = ShipOrientation::Vertical;
                    orientationConfirmed = true;
                    qDebug() << "Orientation confirmed: Vertical";
                }
            }

            // Очищаем очередь и генерируем целевые ходы
            while (!huntMoves.empty()) huntMoves.pop();

            if (orientationConfirmed) {
                generateTargetedHuntMoves();
            }
            else {
                generateHuntMoves(lastHit.first, lastHit.second);
            }
        }
    }
    else {
        qDebug() << "Computer MISS at:" << lastRow << lastCol;

        // При промахе НЕ переключаемся в режим поиска!
        if (currentMode == AIMode::Hunt) {
            qDebug() << "Miss in HUNT mode, continuing to hunt current ship";

            // При промахе можем попробовать сгенерировать новые ходы
            if (huntMoves.empty()) {
                qDebug() << "No hunt moves in queue, generating new ones";
                if (orientationConfirmed) {
                    generateTargetedHuntMoves();
                }
                else {
                    generateHuntMoves(lastHit.first, lastHit.second);
                }
            }
        }
    }
}