#include "GameWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(800, 600);
    resize(1000, 700);

    // Создаем UI
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    statusLabel = new QLabel("Welcome", this);
    mainLayout->addWidget(statusLabel);

    QHBoxLayout* boardsLayout = new QHBoxLayout();
    playerBoardWidget = new BoardWidget(this);
    opponentBoardWidget = new BoardWidget(this);
    boardsLayout->addWidget(playerBoardWidget, 1);
    boardsLayout->addWidget(opponentBoardWidget, 1);
    mainLayout->addLayout(boardsLayout);

    newGameButton = new QPushButton("New game", this);
    mainLayout->addWidget(newGameButton);

    // Создаем игровую логику
    game = new Game(this);

    // Подключаем сигналы
    connect(game, &Game::gameStateChanged, this, &GameWindow::onGameStateChanged);
    connect(game, &Game::gamePhaseChanged, this, &GameWindow::onGamePhaseChanged);
    connect(game, &Game::message, this, &GameWindow::onMessageReceived);
    connect(game, &Game::gameFinished, this, &GameWindow::onGameFinished);

    connect(playerBoardWidget, &BoardWidget::cellClicked,
        this, &GameWindow::onPlayerBoardLeftClick);
    connect(playerBoardWidget, &BoardWidget::rightClick,
        this, &GameWindow::onPlayerBoardRightClick);
    connect(opponentBoardWidget, &BoardWidget::cellClicked,
        this, &GameWindow::onOpponentBoardClick);

    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::onNewGameClicked);

    // Начинаем новую игру
    onNewGameClicked();
}

GameWindow::~GameWindow()
{
    delete game;
}

void GameWindow::onNewGameClicked()
{
    game->startNewGame();
    updateBoards();
}

void GameWindow::onGameStateChanged()
{
    updateBoards();
}

void GameWindow::onGamePhaseChanged(GamePhase newPhase)
{
    // Можно изменить внешний вид в зависимости от фазы
    updateBoards();
}

void GameWindow::onMessageReceived(const QString& message)
{
    statusLabel->setText(message);
}

void GameWindow::onGameFinished(const QString& winner)
{
    statusLabel->setText(winner);
}

void GameWindow::onPlayerBoardLeftClick(int row, int col)
{
    game->processPlayerShipPlacement(row, col);
}

void GameWindow::onPlayerBoardRightClick(int row, int col)
{
    game->processPlayerOrientationChange();
}

void GameWindow::onOpponentBoardClick(int row, int col)
{
    game->processPlayerAttack(row, col);
}

void GameWindow::updateBoards()
{
    playerBoardWidget->setBoardState(game->getPlayerBoardState());
    opponentBoardWidget->setBoardState(game->getOpponentBoardState());
}




