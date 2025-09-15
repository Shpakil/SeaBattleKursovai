#ifndef GAMEOBSERVER_H
#define GAMEOBSERVER_H

#include <QObject>
#include "gamestate.h"

class GameObserver : public QObject
{
    Q_OBJECT

public:
    explicit GameObserver(QObject* parent = nullptr);

public slots:
    virtual void onGameStateChanged(GameState::State state) = 0;
    virtual void onAttackResult(int row, int col, bool hit, bool shipDestroyed) = 0;
    virtual void onGameOver(class Player* winner) = 0;
    virtual void onShipPlaced(int size, int row, int col, class Ship::Direction direction) = 0;
};

#endif // GAMEOBSERVER_H

