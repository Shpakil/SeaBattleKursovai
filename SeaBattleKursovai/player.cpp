#include "player.h"

Player::Player(const QString& name, PlayerType type, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_type(type)
{
}

Player::~Player()
{
}

QString Player::name() const
{
    return m_name;
}

Player::PlayerType Player::type() const
{
    return m_type;
}

Board* Player::board()
{
    return &m_board;
}

const Board* Player::board() const
{
    return &m_board;
}