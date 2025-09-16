#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QVector>
#include "gamestate.h"
#include "gameobserver.h"
#include "player.h"

class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void startNewGame();
    void placeShipsAutomatically();

    GameState* gameState() const;
    Player* currentPlayer() const;
    Player* humanPlayer() const;
    Player* aiPlayer() const;

    bool playerAttack(int row, int col);
    void aiAttack();

    void addObserver(GameObserver* observer);
    void removeObserver(GameObserver* observer);

signals:
    void attackResult(int row, int col, bool hit, bool shipDestroyed);

private slots:
    void onAIMove();
    void onGameStateChanged(GameState::State state);
    void onPlayerMoveMade(int row, int col, bool hit);
    void onShipPlaced(int size, int row, int col, Ship::Direction direction);

private:
    void switchTurn();
    void checkGameOver();
    void notifyObservers();

    GameState* m_gameState;
    Player* m_humanPlayer;
    Player* m_aiPlayer;
    Player* m_currentPlayer;

    QVector<GameObserver*> m_observers;
};

#endif // GAME_H

