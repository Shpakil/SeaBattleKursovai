#include "Player.h"

Player::Player(const std::string& playerName)
    : name(playerName)
{
    //  онструктор инициализирует им€ и игровое поле по умолчанию
}

Board& Player::getBoard()
{
    return board;
}

const std::string& Player::getName() const
{
    return name;
}

bool Player::hasLost() const
{
    // ѕровер€ем, остались ли живые корабли
    return board.allShipsSunk();  // предполагаетс€, что GameBoard умеет провер€ть это
}

