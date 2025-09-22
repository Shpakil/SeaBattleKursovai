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
    void onMoveResultReceived(const QString& result);
    void onGameFinished(const QString& winner);

    void onPlayerBoardLeftClick(int row, int col);
    void onPlayerBoardRightClick(int row, int col);
    void onOpponentBoardClick(int row, int col);
    void onNewGameClicked();

private:
    void updateBoards();
    void updateUIState(); // Новый метод для обновления состояния UI

    Game* game;
    QLabel* statusLabel;
    QPushButton* newGameButton;
    BoardWidget* playerBoardWidget;
    BoardWidget* opponentBoardWidget;

    bool isUILocked; // Флаг блокировки UI
};





