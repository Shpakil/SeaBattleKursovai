#include "game.h"
#include "gamefactory.h"
#include "humanplayer.h"
#include "aiplayer.h"
#include <QTimer>
#include <QDebug>

Game::Game(QObject* parent)
    : QObject(parent)
    , m_gameState(new GameState(this))
    , m_humanPlayer(GameFactory::createPlayer(Player::Human, "Player"))
    , m_aiPlayer(GameFactory::createPlayer(Player::AI))
    , m_currentPlayer(nullptr)
{
    connect(m_gameState, &GameState::stateChanged, this, &Game::onGameStateChanged);
    connect(m_aiPlayer, &Player::moveMade, this, &Game::onPlayerMoveMade);
    connect(m_aiPlayer, &Player::shipPlaced, this, &Game::onShipPlaced);
}

Game::~Game()
{
    delete m_humanPlayer;
    delete m_aiPlayer;
}

void Game::startNewGame()
{
    m_gameState->setState(GameState::Setup);
    m_currentPlayer = m_humanPlayer;

    // Очищаем доски
    delete m_humanPlayer;
    delete m_aiPlayer;

    m_humanPlayer = GameFactory::createPlayer(Player::Human, "Player");
    m_aiPlayer = GameFactory::createPlayer(Player::AI);

    connect(m_aiPlayer, &Player::moveMade, this, &Game::onPlayerMoveMade);
    connect(m_aiPlayer, &Player::shipPlaced, this, &Game::onShipPlaced);
}

void Game::placeShipsAutomatically()
{
    // Расставляем корабли для AI
    m_aiPlayer->placeShips();

    m_gameState->setState(GameState::PlayerTurn);
    m_currentPlayer = m_humanPlayer;
}

GameState* Game::gameState() const
{
    return m_gameState;
}

Player* Game::currentPlayer() const
{
    return m_currentPlayer;
}

Player* Game::humanPlayer() const
{
    return m_humanPlayer;
}

Player* Game::aiPlayer() const
{
    return m_aiPlayer;
}

bool Game::playerAttack(int row, int col)
{
    if (m_gameState->state() != GameState::PlayerTurn ||
        m_currentPlayer != m_humanPlayer) {
        return false;
    }

    Board* aiBoard = m_aiPlayer->board();
    if (!aiBoard->isValidAttack(row, col)) {
        return false;
    }

    bool hit = aiBoard->attack(row, col);
    bool shipDestroyed = hit && aiBoard->cellAt(row, col)->ship() &&
        aiBoard->cellAt(row, col)->ship()->isDestroyed();

    emit attackResult(row, col, hit, shipDestroyed);

    for (GameObserver* observer : m_observers) {
        observer->onAttackResult(row, col, hit, shipDestroyed);
    }

    if (aiBoard->allShipsDestroyed()) {
        m_gameState->setState(GameState::GameOver);
        for (GameObserver* observer : m_observers) {
            observer->onGameOver(m_humanPlayer);
        }
    }
    else {
        switchTurn();
    }

    return hit;
}

void Game::aiAttack()
{
    if (m_gameState->state() != GameState::AITurn ||
        m_currentPlayer != m_aiPlayer) {
        return;
    }

    QTimer::singleShot(1000, this, &Game::onAIMove);
}

void Game::onAIMove()
{
    Board* humanBoard = m_humanPlayer->board();
    m_aiPlayer->makeMove(humanBoard);

    if (humanBoard->allShipsDestroyed()) {
        m_gameState->setState(GameState::GameOver);
        for (GameObserver* observer : m_observers) {
            observer->onGameOver(m_aiPlayer);
        }
    }
    else {
        switchTurn();
    }
}

void Game::onGameStateChanged(GameState::State state)
{
    for (GameObserver* observer : m_observers) {
        observer->onGameStateChanged(state);
    }
}

void Game::onPlayerMoveMade(int row, int col, bool hit)
{
    bool shipDestroyed = hit && m_humanPlayer->board()->cellAt(row, col)->ship() &&
        m_humanPlayer->board()->cellAt(row, col)->ship()->isDestroyed();

    for (GameObserver* observer : m_observers) {
        observer->onAttackResult(row, col, hit, shipDestroyed);
    }
}

void Game::onShipPlaced(int size, int row, int col, Ship::Direction direction)
{
    for (GameObserver* observer : m_observers) {
        observer->onShipPlaced(size, row, col, direction);
    }
}

void Game::switchTurn()
{
    if (m_gameState->state() == GameState::PlayerTurn) {
        m_gameState->setState(GameState::AITurn);
        m_currentPlayer = m_aiPlayer;
        aiAttack();
    }
    else if (m_gameState->state() == GameState::AITurn) {
        m_gameState->setState(GameState::PlayerTurn);
        m_currentPlayer = m_humanPlayer;
    }
}

void Game::checkGameOver()
{
    if (m_humanPlayer->board()->allShipsDestroyed()) {
        m_gameState->setState(GameState::GameOver);
        for (GameObserver* observer : m_observers) {
            observer->onGameOver(m_aiPlayer);
        }
    }
    else if (m_aiPlayer->board()->allShipsDestroyed()) {
        m_gameState->setState(GameState::GameOver);
        for (GameObserver* observer : m_observers) {
            observer->onGameOver(m_humanPlayer);
        }
    }
}

void Game::addObserver(GameObserver* observer)
{
    if (!m_observers.contains(observer)) {
        m_observers.append(observer);
    }
}

void Game::removeObserver(GameObserver* observer)
{
    m_observers.removeAll(observer);
}

void Game::notifyObservers()
{
    for (GameObserver* observer : m_observers) {
        observer->onGameStateChanged(m_gameState->state());
    }
}
