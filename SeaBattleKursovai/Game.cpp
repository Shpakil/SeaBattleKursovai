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
        qDebug() << "Attack rejected - not human's turn!";
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
        qDebug() << "Computer attack rejected - wrong conditions. Phase:"
            << static_cast<int>(currentPhase) << "GameLogic:" << (gameLogic != nullptr);
        return;
    }

    if (gameLogic->isHumanTurn()) {
        qDebug() << "Computer attack rejected - it's human's turn!";
        return;
    }

    // Проверяем, не закончилась ли игра перед ходом компьютера
    if (gameLogic->isGameOver()) {
        qDebug() << "Game is already over before computer move";
        Player* winner = gameLogic->getWinner();
        if (winner) {
            setPhase(GamePhase::GameOver);
            emit gameFinished("Game Over! Winner: " + QString::fromStdString(winner->getName()));
        }
        return;
    }

    // ВАЖНОЕ ИСПРАВЛЕНИЕ: Синхронизируем доступные ходы компьютера с доской игрока
    // Это удалит клетки, которые уже отмечены как промахи после потопления кораблей
    computerPlayer->syncAvailableMovesWithBoard(humanPlayer->getBoard());

    auto move = computerPlayer->makeMove();
    qDebug() << "Computer attacking at:" << move.first << move.second;

    // Проверяем валидность хода
    if (move.first < 0 || move.first >= 10 || move.second < 0 || move.second >= 10) {
        qDebug() << "Invalid computer move! Switching to player turn.";
        setPhase(GamePhase::PlayerTurn);
        emit message("Computer made invalid move. Your turn!");
        return;
    }

    MoveResult result = gameLogic->executeMove(move.first, move.second);
    qDebug() << "Move result:" << static_cast<int>(result);

    QString position = QString("%1%2").arg(QChar('A' + move.second)).arg(move.first + 1);
    QString messageText;

    switch (result) {
    case MoveResult::Invalid:
        messageText = "Invalid move by computer at " + position;
        qDebug() << "Computer made invalid move - switching to player turn";

        // Удаляем невалидный ход из доступных
        computerPlayer->removeMoveFromAvailable(move.first, move.second);

        setPhase(GamePhase::PlayerTurn);
        emit message(messageText + " Your turn!");
        break;

    case MoveResult::Miss:
        messageText = "Computer missed at " + position;
        computerPlayer->updateAIMode(move.first, move.second, false, false);
        setPhase(GamePhase::PlayerTurn);
        emit message(messageText + " Your turn!");
        break;

    case MoveResult::Hit:
        messageText = "Computer hit your ship at " + position;
        computerPlayer->updateAIMode(move.first, move.second, true, false);
        emit message(messageText);
        // Компьютер продолжает стрелять при попадании
        computerActionTimer->start(1000);
        break;

    case MoveResult::Sink:
        messageText = "Computer sank your ship at " + position;
        computerPlayer->updateAIMode(move.first, move.second, true, true);
        emit message(messageText);

        // Синхронизируем доступные ходы после потопления корабля
        // Это удалит клетки вокруг потопленного корабля
        computerPlayer->syncAvailableMovesWithBoard(humanPlayer->getBoard());

        // Проверяем, не закончилась ли игра после потопления корабля
        if (gameLogic->isGameOver()) {
            Player* winner = gameLogic->getWinner();
            if (winner) {
                setPhase(GamePhase::GameOver);
                emit gameFinished("Game Over! Winner: " + QString::fromStdString(winner->getName()));
            }
        }
        else {
            // Игра продолжается - компьютер делает следующий ход
            computerActionTimer->start(1000);
        }
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

    // Дополнительная проверка окончания игры
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
