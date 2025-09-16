#include "humanplayer.h"
#include <QDebug>

HumanPlayer::HumanPlayer(const QString& name, QObject* parent)
    : Player(name, Player::Human, parent)
{
}

void HumanPlayer::placeShips()
{
    // Человек расставляет корабли через UI
    emit shipsPlaced();
}

bool HumanPlayer::makeMove(Board* opponentBoard)
{
    Q_UNUSED(opponentBoard);
    // Ход человека обрабатывается через UI
    return false;
}

void HumanPlayer::placeShipManually(int row, int col, Ship* ship)
{
    if (m_board.placeShip(row, col, ship)) {
        emit shipPlaced(ship->size(), row, col, ship->direction());
    }
}