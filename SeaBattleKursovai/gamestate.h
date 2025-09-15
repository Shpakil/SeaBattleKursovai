#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>

class GameState : public QObject
{
    Q_OBJECT

public:
    enum State {
        Setup,
        PlayerTurn,
        AITurn,
        GameOver
    };
    Q_ENUM(State)

        GameState(QObject* parent = nullptr);

    State state() const;
    void setState(State newState);

signals:
    void stateChanged(State newState);

private:
    State m_state;
};

#endif // GAMESTATE_H
