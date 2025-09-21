#pragma once

#include <QObject>
#include <QString>
#include "GameLogic.h"

// ����� Game ��������� ��������� � �������� � UI ����� �������
class Game : public QObject
{
    Q_OBJECT

private:
    GameLogic* logic;  // ������ ����

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void newGame(Player* p1, Player* p2);   // ������ ����� ����
    void processMove(int row, int col);     // ��������� ���� ������

signals:
    void moveResult(const QString& msg);    // ��������� ��� UI ("���������!" / "����!")
    void boardUpdated();                    // ������ ��� ����������� �����
    void gameOver(const QString& winner);   // ������ ��� ���������� ����
};

