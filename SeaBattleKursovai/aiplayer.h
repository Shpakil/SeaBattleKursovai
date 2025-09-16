#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "player.h"
#include <QVector>
#include <QPoint>

class AIPlayer : public Player
{
    Q_OBJECT

public:
    explicit AIPlayer(QObject* parent = nullptr);

    void placeShips() override;
    bool makeMove(Board* opponentBoard) override;

private:
    void placeShipsAutomatically();
    QPoint findSmartMove(Board* opponentBoard);

    QVector<QPoint> m_previousMoves;
    QVector<QPoint> m_targetMoves;
};

#endif // AIPLAYER_H

