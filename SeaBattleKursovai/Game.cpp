#include "Game.h"

Game::Game(QObject* parent): QObject(parent), logic(nullptr)
{
}

Game::~Game()
{
    delete logic;
}

void Game::newGame(Player* p1, Player* p2)
{
    delete logic; //освобождаем старую игру
    logic = new GameLogic(p1, p2);

    emit moveResult("=== Ќова€ игра началась! ===");
    emit boardUpdated();
}

void Game::processMove(int row, int col)
{
    if (!logic || logic->isGameOver())
        return;

    bool hit = logic->makeTurn(row, col);

    if (hit)
        emit moveResult("Hit!");
    else
        emit moveResult("Miss!");

    emit boardUpdated();

    if (logic->isGameOver())
    {
        QString winner = QString::fromStdString(logic->getWinner()->getName());
        emit gameOver("Winner: " + winner);
    }
}


