#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"

class HumanPlayer : public Player
{
    Q_OBJECT

public:
    explicit HumanPlayer(const QString& name, QObject* parent = nullptr);

    void placeShips() override;
    bool makeMove(Board* opponentBoard) override;

    void placeShipManually(int row, int col, Ship* ship);
};

#endif // HUMANPLAYER_H

