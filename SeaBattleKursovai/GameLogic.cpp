#include "GameLogic.h"
#include <QDebug>

GameLogic::GameLogic(Player* human, Player* computer)
    : humanPlayer(human), computerPlayer(computer),
    currentPlayer(human), opponentPlayer(computer)  // Человек ходит первым
{
    qDebug() << "GameLogic initialized. First player: Human";
}

bool GameLogic::isValidMove(int row, int col) const
{
    // Проверяем границы
    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        qDebug() << "Move invalid: coordinates out of bounds";
        return false;
    }

    // Проверяем, что по этой клетке еще не стреляли
    const Cell& targetCell = opponentPlayer->getBoard().getCell(row, col);
    bool valid = !targetCell.isHit() && !targetCell.isMiss();

    qDebug() << "Move validation:" << (valid ? "VALID" : "INVALID")
        << "at" << row << col
        << "(hit:" << targetCell.isHit() << "miss:" << targetCell.isMiss() << ")";

    return valid;
}

MoveResult GameLogic::executeMove(int row, int col)
{
    qDebug() << "=== EXECUTING MOVE ===";
    qDebug() << "Attacker:" << currentPlayer->getName().c_str();
    qDebug() << "Target:" << opponentPlayer->getName().c_str();
    qDebug() << "Coordinates:" << row << col;

    if (!isValidMove(row, col)) {
        qDebug() << "Move is invalid, aborting";
        return MoveResult::Invalid;
    }

    if (isGameOver()) {
        qDebug() << "Game is over, cannot execute move";
        return MoveResult::GameOver;
    }

    // Стреляем по противнику
    Cell& targetCell = opponentPlayer->getBoard().getCell(row, col);
    bool hit = targetCell.shoot();

    qDebug() << "Shot result: " << (hit ? "HIT" : "MISS");

    // Проверяем, не закончилась ли игра после выстрела
    if (isGameOver()) {
        qDebug() << "Game over after this move";
        return MoveResult::GameOver;
    }

    if (hit) {
        // Проверяем, потоплен ли корабль
        bool sankShip = false;
        for (const auto& ship : opponentPlayer->getBoard().getShips()) {
            if (ship.isSunk()) {
                sankShip = true;
                qDebug() << "Ship SUNK!";
                break;
            }
        }
        return sankShip ? MoveResult::Sink : MoveResult::Hit;
    }
    else {
        // При промахе передаем ход
        qDebug() << "Miss - switching turns";
        switchTurns();
        return MoveResult::Miss;
    }
}

bool GameLogic::isGameOver() const
{
    bool humanLost = humanPlayer->getBoard().allShipsSunk();
    bool computerLost = computerPlayer->getBoard().allShipsSunk();

    if (humanLost || computerLost) {
        qDebug() << "Game over - Human lost:" << humanLost << "Computer lost:" << computerLost;
        return true;
    }
    return false;
}

Player* GameLogic::getWinner() const
{
    if (humanPlayer->getBoard().allShipsSunk()) {
        qDebug() << "Winner: Computer";
        return computerPlayer;
    }
    if (computerPlayer->getBoard().allShipsSunk()) {
        qDebug() << "Winner: Human";
        return humanPlayer;
    }
    return nullptr;
}

void GameLogic::switchTurns()
{
    qDebug() << "SWITCHING TURNS:";
    qDebug() << "Before - Current:" << currentPlayer->getName().c_str()
        << "Opponent:" << opponentPlayer->getName().c_str();

    // Меняем игроков местами
    Player* temp = currentPlayer;
    currentPlayer = opponentPlayer;
    opponentPlayer = temp;

    qDebug() << "After - Current:" << currentPlayer->getName().c_str()
        << "Opponent:" << opponentPlayer->getName().c_str();
}

