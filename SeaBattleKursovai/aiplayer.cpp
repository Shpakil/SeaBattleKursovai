#include "aiplayer.h"
#include "gamefactory.h"
#include <QRandomGenerator>
#include <QDebug>

AIPlayer::AIPlayer(QObject* parent)
    : Player("Computer", Player::AI, parent)
{
}

void AIPlayer::placeShips()
{
    placeShipsAutomatically();
    emit shipsPlaced();
}

bool AIPlayer::makeMove(Board* opponentBoard)
{
    if (!opponentBoard) return false;

    QPoint move = findSmartMove(opponentBoard);
    bool hit = opponentBoard->attack(move.x(), move.y());

    m_previousMoves.append(move);

    if (hit) {
        // Добавляем соседние клетки как цели
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                if (dr != 0 && dc != 0) continue; // Только по горизонтали/вертикали

                int newRow = move.x() + dr;
                int newCol = move.y() + dc;

                if (opponentBoard->isValidAttack(newRow, newCol)) {
                    m_targetMoves.append(QPoint(newRow, newCol));
                }
            }
        }
    }

    emit moveMade(move.x(), move.y(), hit);
    return hit;
}

void AIPlayer::placeShipsAutomatically()
{
    QVector<int> shipSizes = GameFactory::defaultShipSizes();

    for (int size : shipSizes) {
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 100) {
            attempts++;

            int row = QRandomGenerator::global()->bounded(Board::SIZE);
            int col = QRandomGenerator::global()->bounded(Board::SIZE);
            Ship::Direction direction = QRandomGenerator::global()->bounded(2) ?
                Ship::Horizontal : Ship::Vertical;

            Ship* ship = new Ship(size, direction);
            if (board()->placeShip(row, col, ship)) {
                placed = true;
                emit shipPlaced(size, row, col, direction);
            }
            else {
                delete ship;
            }
        }
    }
}

QPoint AIPlayer::findSmartMove(Board* opponentBoard)
{
    // Если есть приоритетные цели
    if (!m_targetMoves.isEmpty()) {
        return m_targetMoves.takeFirst();
    }

    // Иначе случайный ход
    QVector<QPoint> possibleMoves;

    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            if (opponentBoard->isValidAttack(row, col)) {
                possibleMoves.append(QPoint(row, col));
            }
        }
    }

    if (!possibleMoves.isEmpty()) {
        int index = QRandomGenerator::global()->bounded(possibleMoves.size());
        return possibleMoves[index];
    }

    return QPoint(-1, -1);
}
