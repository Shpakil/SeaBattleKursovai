#pragma once
#include "Player.h"
#include "Board.h"
#include "Ship.h"

enum class MoveResult {
    Invalid,    // Invalid move
    Miss,       // Missed shot
    Hit,        // Hit a ship
    Sink,       // Sank a ship
    GameOver    // Game ended with this move
};

class GameLogic
{
private:
    Player* humanPlayer;
    Player* computerPlayer;
    Player* currentPlayer;    // Тот, кто сейчас ходит
    Player* opponentPlayer;   // Тот, в кого стреляют

public:
    GameLogic(Player* human, Player* computer);

    // Core game logic
    bool isValidMove(int row, int col) const;
    MoveResult executeMove(int row, int col);
    bool isGameOver() const;
    Player* getWinner() const;
    Player* getCurrentPlayer() const { return currentPlayer; }
    Player* getOpponentPlayer() const { return opponentPlayer; }
    void switchTurns();

    // Helper methods
    bool isHumanTurn() const { return currentPlayer == humanPlayer; }
};
