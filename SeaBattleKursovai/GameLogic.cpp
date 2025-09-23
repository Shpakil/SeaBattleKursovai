#include "GameLogic.h"
#include <QDebug>

GameLogic::GameLogic(Player* human, Player* computer)
    : humanPlayer(human), computerPlayer(computer),
    currentPlayer(human), opponentPlayer(computer)
{
    qDebug() << "GameLogic initialized. First player: Human";
}

bool GameLogic::isValidMove(int row, int col) const
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        return false;
    }

    const Cell& targetCell = opponentPlayer->getBoard().getCell(row, col);
    bool valid = !targetCell.isHit() && !targetCell.isMiss();

    qDebug() << "Move validation at" << row << col << ":" << valid;

    return valid;
}

MoveResult GameLogic::executeMove(int row, int col)
{
    qDebug() << "=== EXECUTING MOVE ===";
    qDebug() << "Attacker:" << currentPlayer->getName().c_str();
    qDebug() << "Target:" << opponentPlayer->getName().c_str();

    if (!isValidMove(row, col)) {
        return MoveResult::Invalid;
    }

    if (isGameOver()) {
        return MoveResult::GameOver;
    }

    // Стреляем по противнику
    Cell& targetCell = opponentPlayer->getBoard().getCell(row, col);
    bool hit = targetCell.shoot();

    qDebug() << "Shot result: " << (hit ? "HIT" : "MISS");

    if (isGameOver()) {
        return MoveResult::GameOver;
    }

    if (hit) {
        // Находим корабль, в который попали, и проверяем, потоплен ли он
        bool sankShip = false;
        const Ship* sunkShipPtr = nullptr;

        // Ищем корабль, содержащий пораженную клетку
        for (const auto& ship : opponentPlayer->getBoard().getShips()) {
            for (auto* shipCell : ship.getCells()) {
                if (shipCell->getRow() == row && shipCell->getCol() == col) {
                    // Нашли корабль - проверяем, потоплен ли он теперь
                    if (ship.isSunk()) {
                        sankShip = true;
                        sunkShipPtr = &ship;
                        qDebug() << "Ship SUNK!";
                    }
                    break;
                }
            }
            if (sankShip) break;
        }

        if (sankShip && sunkShipPtr) {
            // Отмечаем область вокруг потопленного корабля
            opponentPlayer->getBoard().markAreaAroundSunkShip(*sunkShipPtr);
            return MoveResult::Sink;
        }

        return MoveResult::Hit;
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

    qDebug() << "Game over check - Human lost:" << humanLost << "Computer lost:" << computerLost;

    return humanLost || computerLost;
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
    qDebug() << "No winner yet";
    return nullptr;
}


void GameLogic::switchTurns()
{
    qDebug() << "SWITCHING TURNS";

    if (currentPlayer == humanPlayer) {
        currentPlayer = computerPlayer;
        opponentPlayer = humanPlayer;
    }
    else {
        currentPlayer = humanPlayer;
        opponentPlayer = computerPlayer;
    }

    qDebug() << "Now current:" << currentPlayer->getName().c_str();
    qDebug() << "Now opponent:" << opponentPlayer->getName().c_str();
}


