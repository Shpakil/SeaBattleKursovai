#include "Player.h"

Player::Player(const std::string& playerName)
    : name(playerName), board(10, 10)
{
}

void Player::reset()
{
    board = Board(10, 10);
}

Board& Player::getBoard()
{
    return board;
}

const Board& Player::getBoard() const
{
    return board;
}

const std::string& Player::getName() const
{
    return name;
}

bool Player::hasLost() const
{
    return board.allShipsSunk();
}

