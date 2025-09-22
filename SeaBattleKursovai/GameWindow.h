#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "BoardWidget.h"
#include "Game.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget* parent = nullptr);
    ~GameWindow();

private slots:
    void onGameStateChanged();
    void onGamePhaseChanged(GamePhase newPhase);
    void onMessageReceived(const QString& message);
    void onGameFinished(const QString& winner);

    void onPlayerBoardLeftClick(int row, int col);
    void onPlayerBoardRightClick(int row, int col);
    void onOpponentBoardClick(int row, int col);
    void onNewGameClicked();

private:
    void updateBoards();

    Game* game;
    QLabel* statusLabel;
    QPushButton* newGameButton;
    BoardWidget* playerBoardWidget;
    BoardWidget* opponentBoardWidget;
};






