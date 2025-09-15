#include "gamestate.h"

GameState::GameState(QObject* parent)
    : QObject(parent)
    , m_state(Setup)
{
}

GameState::State GameState::state() const
{
    return m_state;
}

void GameState::setState(State newState)
{
    if (m_state != newState) {
        m_state = newState;
        emit stateChanged(newState);
    }
}