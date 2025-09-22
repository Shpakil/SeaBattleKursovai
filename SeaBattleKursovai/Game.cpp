#include "Game.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

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

// Добавьте этот метод
void Game::setPhase(GamePhase newPhase)
{
    if (currentPhase != newPhase) {
        qDebug() << "Phase change:" << static_cast<int>(currentPhase) << "->" << static_cast<int>(newPhase);
        currentPhase = newPhase;
        emit gamePhaseChanged(newPhase);
    }
}

void Game::startNewGame()
{
    qDebug() << "=== STARTING NEW GAME ===";

    delete gameLogic;
    gameLogic = nullptr;

    humanPlayer->reset();
    computerPlayer->reset();

    setPhase(GamePhase::ShipPlacement);
    emit message("Place your ships. " + humanPlayer->getCurrentShipDescription());
}

void Game::processPlayerShipPlacement(int row, int col)
{
    if (currentPhase != GamePhase::ShipPlacement) {
        qDebug() << "Ship placement rejected - wrong phase:" << static_cast<int>(currentPhase);
        return;
    }

    qDebug() << "Processing ship placement at:" << row << col;

    if (humanPlayer->placeShip(row, col)) {
        emit gameStateChanged();

        if (humanPlayer->isPlacementComplete()) {
            emit message("All ships placed! Computer is placing its ships...");
            setPhase(GamePhase::ComputerPlacement);
            QTimer::singleShot(100, this, &Game::executeComputerShipPlacement);
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
    qDebug() << "=== PROCESSING PLAYER ATTACK ===";

    if (currentPhase != GamePhase::PlayerTurn) {
        qDebug() << "Attack rejected - wrong phase:" << static_cast<int>(currentPhase);
        emit message("Not your turn!");
        return;
    }

    if (!gameLogic) {
        qDebug() << "Attack rejected - game logic not initialized";
        emit message("Game not ready!");
        return;
    }

    if (!gameLogic->isHumanTurn()) {
        qDebug() << "Attack rejected - not human's turn! Current player:"
            << gameLogic->getCurrentPlayer()->getName().c_str();
        emit message("Wait for computer's move!");
        return;
    }

    qDebug() << "Player attacking at:" << row << col;

    MoveResult result = gameLogic->executeMove(row, col);

    qDebug() << "Move result:" << static_cast<int>(result);

    switch (result) {
    case MoveResult::Invalid:
        emit message("Invalid move! Choose a different cell.");
        return;

    case MoveResult::Miss:
        emit message("Miss! Computer's turn.");
        setPhase(GamePhase::ComputerTurn);
        computerActionTimer->start(1000);
        break;

    case MoveResult::Hit:
        emit message("Hit! Shoot again.");
        break;

    case MoveResult::Sink:
        emit message("You sank a ship! Shoot again.");
        break;

    case MoveResult::GameOver:
    {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Game Over! Winner: " + QString::fromStdString(winner->getName()));
        }
        return;
    }
    }

    emit gameStateChanged();

    if (gameLogic->isGameOver()) {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Game Over! Winner: " + QString::fromStdString(winner->getName()));
        }
    }
}

void Game::executeComputerShipPlacement()
{
    qDebug() << "Computer placing ships...";
    computerPlayer->placeShips();
    startBattlePhase();
}

void Game::startBattlePhase()
{
    qDebug() << "=== STARTING BATTLE PHASE ===";
    gameLogic = new GameLogic(humanPlayer, computerPlayer);
    setPhase(GamePhase::PlayerTurn);
    emit message("Battle started! Your turn. Click on computer's board.");
    emit gameStateChanged();
}

void Game::executeComputerAttack()
{
    qDebug() << "=== COMPUTER ATTACK ===";

    if (currentPhase != GamePhase::ComputerTurn || !gameLogic) {
        qDebug() << "Computer attack rejected - wrong conditions";
        return;
    }

    if (gameLogic->isHumanTurn()) {
        qDebug() << "Computer attack rejected - it's human's turn!";
        return;
    }

    auto move = computerPlayer->makeMove();
    qDebug() << "Computer attacking at:" << move.first << move.second;

    MoveResult result = gameLogic->executeMove(move.first, move.second);

    QString position = QString("%1%2").arg(QChar('A' + move.second)).arg(move.first + 1);
    QString messageText;

    switch (result) {
    case MoveResult::Miss:
        messageText = "Computer missed at " + position;
        setPhase(GamePhase::PlayerTurn);
        emit message(messageText + " Your turn!");
        break;

    case MoveResult::Hit:
        messageText = "Computer hit your ship at " + position;
        emit message(messageText);
        break;

    case MoveResult::Sink:
        messageText = "Computer sank your ship at " + position;
        emit message(messageText);
        break;

    case MoveResult::GameOver:
    {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Game Over! Winner: " + QString::fromStdString(winner->getName()));
        }
        return;
    }
    default:
        messageText = "Computer made a move";
    }

    emit message(messageText);
    emit gameStateChanged();

    if (result == MoveResult::Miss) {
        qDebug() << "Computer missed - returning turn to human";
    }
    else {
        qDebug() << "Computer hit - shooting again";
        computerActionTimer->start(1000);
    }

    if (gameLogic->isGameOver()) {
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Game Over! Winner: " + QString::fromStdString(winner->getName()));
        }
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
        return "Your turn - attack computer!";
    case GamePhase::ComputerTurn:
        return "Computer's turn...";
    case GamePhase::GameOver:
        return "Game Over!";
    default:
        return "Unknown";
    }
}


