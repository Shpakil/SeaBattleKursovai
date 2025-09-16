#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include "board.h"

class Player : public QObject
{
    Q_OBJECT

public:
    enum PlayerType {
        Human,
        AI
    };

    Player(const QString& name, PlayerType type, QObject* parent = nullptr);
    virtual ~Player();

    QString name() const;
    PlayerType type() const;
    Board* board();
    const Board* board() const;

    virtual void placeShips() = 0;
    virtual bool makeMove(Board* opponentBoard) = 0;

signals:
    void shipsPlaced();
    void moveMade(int row, int col, bool hit);

protected:
    QString m_name;
    PlayerType m_type;
    Board m_board;
};

#endif // PLAYER_H
#ifndef PLAYER_H  
#define PLAYER_H  

#include <QObject>  
#include "board.h"  

class Player : public QObject  
{  
   Q_OBJECT  

public:  
   explicit Player(QObject* parent = nullptr);  
   virtual ~Player();  

   virtual void placeShips() = 0;  
   virtual void makeMove(Board* opponentBoard) = 0;  

signals:  
   void moveMade(int row, int col, bool hit);  
   void shipPlaced(int size, int row, int col, Ship::Direction direction); // Добавляем сигнал shipPlaced  
};  

#endif // PLAYER_H
