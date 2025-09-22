#include "GameWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent), isUILocked(false)
{
    setMinimumSize(800, 600);
    resize(1000, 700);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    statusLabel = new QLabel("Welcome to Battleship!", this);
    mainLayout->addWidget(statusLabel);

    QHBoxLayout* boardsLayout = new QHBoxLayout();
    playerBoardWidget = new BoardWidget(this);
    opponentBoardWidget = new BoardWidget(this);

    playerBoardWidget->setObjectName("PlayerBoard");
    opponentBoardWidget->setObjectName("OpponentBoard");

    boardsLayout->addWidget(playerBoardWidget, 1);
    boardsLayout->addWidget(opponentBoardWidget, 1);
    mainLayout->addLayout(boardsLayout);

    newGameButton = new QPushButton("New Game", this);
    mainLayout->addWidget(newGameButton);

    game = new Game(this);

    // ���������� �������
    connect(game, &Game::gameStateChanged, this, &GameWindow::onGameStateChanged);
    connect(game, &Game::gamePhaseChanged, this, &GameWindow::onGamePhaseChanged);
    connect(game, &Game::message, this, &GameWindow::onMessageReceived);
    connect(game, &Game::moveResult, this, &GameWindow::onMoveResultReceived);
    connect(game, &Game::gameFinished, this, &GameWindow::onGameFinished);

    connect(playerBoardWidget, &BoardWidget::cellClicked,
        this, &GameWindow::onPlayerBoardLeftClick);
    connect(playerBoardWidget, &BoardWidget::rightClick,
        this, &GameWindow::onPlayerBoardRightClick);
    connect(opponentBoardWidget, &BoardWidget::cellClicked,
        this, &GameWindow::onOpponentBoardClick);

    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::onNewGameClicked);

    onNewGameClicked();
}

GameWindow::~GameWindow()
{
    delete game;
}

void GameWindow::onNewGameClicked()
{
    isUILocked = false;
    game->startNewGame();
    updateUIState();
    updateBoards();
}

void GameWindow::onGameStateChanged()
{
    // ��� ���������� ��������� ���� ������������ UI
    // (�������� ����� ��� ���������, ����� ��� �������� � ������)
    if (game->getCurrentPhase() == GamePhase::PlayerTurn) {
        isUILocked = false;
        qDebug() << "UI unlocked due to game state change";
    }
    updateUIState();
    updateBoards();
}

void GameWindow::onGamePhaseChanged(GamePhase newPhase)
{
    qDebug() << "Game phase changed to:" << static_cast<int>(newPhase);

    // ������������ UI ��� �������� � ���� ������
    if (newPhase == GamePhase::PlayerTurn) {
        isUILocked = false;
        qDebug() << "UI unlocked for player turn";
    }

    updateUIState();
    updateBoards();
}

void GameWindow::onMessageReceived(const QString& message)
{
    statusLabel->setText(message);
    qDebug() << "Game message:" << message;
}

void GameWindow::onMoveResultReceived(const QString& result)
{
    qDebug() << "Move result:" << result;

    // ��� ��������� ����� ������������ UI ��� ���������� ����
    if (result == "Hit!" || result == "Sank!") {
        isUILocked = false;
        qDebug() << "UI unlocked after hit/sank";
        updateUIState();
    }
}

void GameWindow::onGameFinished(const QString& winner)
{
    statusLabel->setText(winner);
    isUILocked = false;
    updateUIState();
    qDebug() << "Game finished:" << winner;
}

void GameWindow::onPlayerBoardLeftClick(int row, int col)
{
    if (isUILocked) {
        qDebug() << "UI locked - ignoring player board click";
        return;
    }

    qDebug() << "Player board click - Placement at:" << row << col;
    game->processPlayerShipPlacement(row, col);
}

void GameWindow::onPlayerBoardRightClick(int row, int col)
{
    if (isUILocked) {
        qDebug() << "UI locked - ignoring player board right click";
        return;
    }

    qDebug() << "Player board right click - Orientation change";
    game->processPlayerOrientationChange();
}

void GameWindow::onOpponentBoardClick(int row, int col)
{
    if (isUILocked) {
        qDebug() << "UI locked - ignoring opponent board click";
        return;
    }

    qDebug() << "Opponent board click - Attack at:" << row << col;

    // ��������� UI ������ �� ����� ��������� ����� ����
    // ��� ��������� ������������ � onMoveResultReceived
    // ��� ������� ������������ � onGamePhaseChanged (����� ��� �������� � ������)
    isUILocked = true;
    updateUIState();

    game->processPlayerAttack(row, col);
}

void GameWindow::updateBoards()
{
    auto playerState = game->getPlayerBoardState();
    auto opponentState = game->getOpponentBoardState();

    playerBoardWidget->setBoardState(playerState);
    opponentBoardWidget->setBoardState(opponentState);

    playerBoardWidget->update();
    opponentBoardWidget->update();
}

void GameWindow::updateUIState()
{
    // ��������� ��� ������������ �������� UI
    bool canPlaceShips = !isUILocked && game->getCurrentPhase() == GamePhase::ShipPlacement;
    bool canAttack = !isUILocked && game->getCurrentPhase() == GamePhase::PlayerTurn;

    playerBoardWidget->setEnabled(canPlaceShips);
    opponentBoardWidget->setEnabled(canAttack);
    newGameButton->setEnabled(!isUILocked);

    // ���������� ���������
    if (isUILocked) {
        playerBoardWidget->setStyleSheet("background-color: #f0f0f0;");
        opponentBoardWidget->setStyleSheet("background-color: #f0f0f0;");
    }
    else {
        playerBoardWidget->setStyleSheet("");
        opponentBoardWidget->setStyleSheet("");
    }
}



