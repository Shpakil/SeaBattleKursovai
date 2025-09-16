#include "gamefactory.h"
#include "humanplayer.h"
#include "aiplayer.h"
#include "game.h"

Player* GameFactory::createPlayer(Player::PlayerType type, const QString& name)
{
    switch (type) {
    case Player::Human:
        return new HumanPlayer(name);
    case Player::AI:
        return new AIPlayer();
    default:
        return nullptr;
    }
}

Game* GameFactory::createGame(QObject* parent)
{
    return new Game(parent);
}

QVector<int> GameFactory::defaultShipSizes()
{
    return { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
}
