#pragma once

#include <QObject>
#include <QString>
#include "GameLogic.h"

//  ласс Game управл€ет процессом и общаетс€ с UI через сигналы
class Game : public QObject
{
    Q_OBJECT

private:
    GameLogic* logic;  // Ћогика игры

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void newGame(Player* p1, Player* p2);   // запуск новой игры
    void processMove(int row, int col);     // обработка хода игрока

signals:
    void moveResult(const QString& msg);    // сообщение дл€ UI ("ѕопадание!" / "ћимо!")
    void boardUpdated();                    // сигнал дл€ перерисовки доски
    void gameOver(const QString& winner);   // сигнал при завершении игры
};

