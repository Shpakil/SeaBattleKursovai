#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include <QObject>
#include "player.h"

class GameFactory
{
public:
    static Player* createPlayer(Player::PlayerType type, const QString& name = "");
    static class Game* createGame(QObject* parent = nullptr);

    static QVector<int> defaultShipSizes();
};

#endif // GAMEFACTORY_H

