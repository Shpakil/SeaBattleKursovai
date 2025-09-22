#include "Game.h"
#include <QCoreApplication>
#include <QTimer>

Game::Game(QObject* parent)
    : QObject(parent), currentPhase(GamePhase::ShipPlacement),
    gameLogic(nullptr), computerActionTimer(new QTimer(this))
{
    humanPlayer = new HumanPlayer("Player");
    computerPlayer = new ComputerPlayer("Computer");
    computerActionTimer->setSingleShot(true);

    connect(computerActionTimer, &QTimer::timeout, this, &Game::executeComputerAttack);
}

Game::~Game()
{
    delete gameLogic;
    delete humanPlayer;
    delete computerPlayer;
}

void Game::startNewGame()
{
    delete gameLogic;
    gameLogic = nullptr;

    humanPlayer->reset();
    computerPlayer->reset();

    setPhase(GamePhase::ShipPlacement);
    emit message(humanPlayer->getCurrentShipDescription());
}

void Game::processPlayerShipPlacement(int row, int col)
{
    if (currentPhase != GamePhase::ShipPlacement) return;

    if (humanPlayer->placeShip(row, col)) {
        emit gameStateChanged();

        if (humanPlayer->isPlacementComplete()) {
            emit message("All ships placed! Computer is placing its ships...");
            setPhase(GamePhase::ComputerPlacement);
            QTimer::singleShot(500, this, &Game::executeComputerShipPlacement);
        }
        else {
            emit message(humanPlayer->getCurrentShipDescription());
        }
    }
    else {
        emit message("Cannot place ship here. " + humanPlayer->getCurrentShipDescription());
    }
}

void Game::processPlayerOrientationChange()
{
    if (currentPhase != GamePhase::ShipPlacement) return;

    humanPlayer->toggleOrientation();
    emit message(humanPlayer->getCurrentShipDescription());
}

void Game::processPlayerAttack(int row, int col)
{
    if (currentPhase != GamePhase::PlayerTurn) return;

    if (!gameLogic) return;

    MoveResult result = gameLogic->executeMove(row, col);

    switch (result) {
    case MoveResult::Invalid:
        emit message("Invalid move! Try a different cell.");
        return;
    case MoveResult::Miss:
        emit moveResult("Miss!");
        break;
    case MoveResult::Hit:
        emit moveResult("Hit!");
        break;
    case MoveResult::Sink:
        emit moveResult("You sank a ship!");
        break;
    case MoveResult::GameOver:
    {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Winner: " + QString::fromStdString(winner->getName()));
        }
        return;
    }
    }

    emit gameStateChanged();

    if (gameLogic->isGameOver()) {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Winner: " + QString::fromStdString(winner->getName()));
        }
    }
    else {
        setPhase(GamePhase::ComputerTurn);
        computerActionTimer->start(1000);
    }
}

void Game::executeComputerShipPlacement()
{
    computerPlayer->placeShips();
    startBattlePhase();
}

void Game::startBattlePhase()
{
    gameLogic = new GameLogic(humanPlayer, computerPlayer);
    setPhase(GamePhase::PlayerTurn);
    emit message("Computer has placed ships. Your turn! Click on opponent's board.");
    emit gameStateChanged();
}

void Game::executeComputerAttack()
{
    if (currentPhase != GamePhase::ComputerTurn || !gameLogic) return;

    auto move = computerPlayer->makeMove();
    MoveResult result = gameLogic->executeMove(move.first, move.second);

    switch (result) {
    case MoveResult::Miss:
        emit message("Computer missed.");
        break;
    case MoveResult::Hit:
        emit message("Computer hit your ship!");
        break;
    case MoveResult::Sink:
        emit message("Computer sank your ship!");
        break;
    case MoveResult::GameOver:
    {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Winner: " + QString::fromStdString(winner->getName()));
        }
        return;
    }
    default:
        break;
    }

    emit gameStateChanged();

    if (gameLogic->isGameOver()) {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Winner: " + QString::fromStdString(winner->getName()));
        }
    }
    else {
        setPhase(GamePhase::PlayerTurn);
        emit message("Your turn! Select a cell on opponent's board.");
    }
}

void Game::setPhase(GamePhase newPhase)
{
    if (currentPhase != newPhase) {
        currentPhase = newPhase;
        emit gamePhaseChanged(newPhase);
    }
}

std::vector<std::vector<int>> Game::getPlayerBoardState() const
{
    return humanPlayer->getBoard().getStateGrid();
}

std::vector<std::vector<int>> Game::getOpponentBoardState() const
{
    auto state = computerPlayer->getBoard().getStateGrid();
    for (auto& row : state) {
        for (auto& cell : row) {
            if (cell == 1) cell = 0;
        }
    }
    return state;
}

QString Game::getGameStatus() const
{
    switch (currentPhase) {
    case GamePhase::ShipPlacement:
        return humanPlayer->getCurrentShipDescription();
    case GamePhase::ComputerPlacement:
        return "Computer is placing ships...";
    case GamePhase::PlayerTurn:
        return "Your turn!";
    case GamePhase::ComputerTurn:
        return "Computer's turn...";
    case GamePhase::GameOver:
        return "Game Over!";
    default:
        return "Unknown state";
    }
}


