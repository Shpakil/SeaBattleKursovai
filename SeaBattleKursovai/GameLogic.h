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
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    Player* opponentPlayer;

public:
    GameLogic(Player* p1, Player* p2);

    bool isValidMove(int row, int col) const;
    MoveResult executeMove(int row, int col);
    bool isGameOver() const;
    Player* getWinner() const;
    Player* getCurrentPlayer() const { return currentPlayer; }
    void switchTurns();

    const Board& getPlayerBoard(Player* player) const;
};
