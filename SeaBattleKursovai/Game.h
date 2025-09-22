#pragma once
#include <QObject>
#include <QTimer>
#include "GameLogic.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

enum class GamePhase {
    ShipPlacement,      // Player placing ships
    ComputerPlacement,  // Computer placing ships
    PlayerTurn,         // Player's attack turn
    ComputerTurn,       // Computer's attack turn
    GameOver           // Game finished
};

class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void startNewGame();
    void processPlayerShipPlacement(int row, int col);
    void processPlayerOrientationChange();
    void processPlayerAttack(int row, int col);

    GamePhase getCurrentPhase() const { return currentPhase; }
    QString getGameStatus() const;
    std::vector<std::vector<int>> getPlayerBoardState() const;
    std::vector<std::vector<int>> getOpponentBoardState() const;

signals:
    void gameStateChanged();
    void gamePhaseChanged(GamePhase newPhase);
    void message(const QString& text);
    void moveResult(const QString& result);
    void gameFinished(const QString& winner);

private slots:
    void executeComputerShipPlacement();
    void executeComputerAttack();

private:
    void setPhase(GamePhase newPhase);
    void startBattlePhase();

    GamePhase currentPhase;
    HumanPlayer* humanPlayer;
    ComputerPlayer* computerPlayer;
    GameLogic* gameLogic;
    QTimer* computerActionTimer;
};

