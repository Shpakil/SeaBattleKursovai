#include "GameLogic.h"
#include <iostream>

GameLogic::GameLogic(Player* p1, Player* p2)
    : player1(p1), player2(p2), currentPlayer(p1), opponentPlayer(p2)
{
}

bool GameLogic::isValidMove(int row, int col) const
{
    if (row < 0 || row >= 10 || col < 0 || col >= 10) return false;

    const Cell& targetCell = opponentPlayer->getBoard().getCell(row, col);
    return !targetCell.isHit() && !targetCell.isMiss();
}

MoveResult GameLogic::executeMove(int row, int col)
{
    if (!isValidMove(row, col)) return MoveResult::Invalid;
    if (isGameOver()) return MoveResult::GameOver;

    Cell& targetCell = opponentPlayer->getBoard().getCell(row, col);
    bool hit = targetCell.shoot();

    if (isGameOver()) return MoveResult::GameOver;

    if (hit) {
        // Check if this hit sank a ship
        bool sankShip = false;
        for (const auto& ship : opponentPlayer->getBoard().getShips()) {
            if (ship.isSunk()) {
                sankShip = true;
                break;
            }
        }
        return sankShip ? MoveResult::Sink : MoveResult::Hit;
    }
    else {
        switchTurns();
        return MoveResult::Miss;
    }
}

bool GameLogic::isGameOver() const
{
    return player1->getBoard().allShipsSunk() || player2->getBoard().allShipsSunk();
}

Player* GameLogic::getWinner() const
{
    if (player1->getBoard().allShipsSunk()) return player2;
    if (player2->getBoard().allShipsSunk()) return player1;
    return nullptr;
}

void GameLogic::switchTurns()
{
    std::swap(currentPlayer, opponentPlayer);
}

const Board& GameLogic::getPlayerBoard(Player* player) const
{
    return player->getBoard();
}

