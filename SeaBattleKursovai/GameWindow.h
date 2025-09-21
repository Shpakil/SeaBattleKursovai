#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "BoardWidget.h"
#include "Game.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

class GameWindow : public QWidget
{
    Q_OBJECT

private:
    Game* game;                  // Логика игры
    BoardWidget* boardWidget;    // Доска игрока
    QLabel* infoLabel;           // Метка для сообщений
    QPushButton* newGameButton;  // Кнопка новой игры

    Player* player1;
    Player* player2;

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow();

private slots:
    void handleCellClicked(int row, int col);
    void updateBoard();
    void showMoveResult(const QString& msg);
    void showGameOver(const QString& winner);
};


