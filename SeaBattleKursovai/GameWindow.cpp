#include "GameWindow.h"

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent)
{
    // --- �������� ���������� ---
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    infoLabel = new QLabel("New Game!", this);   // ��������� �� ����������
    infoLabel->setWordWrap(true);               // ��������� ������� �����
    mainLayout->addWidget(infoLabel);

    boardWidget = new BoardWidget(this);
    mainLayout->addWidget(boardWidget);

    newGameButton = new QPushButton("New Game", this);
    mainLayout->addWidget(newGameButton);

    // --- �������� ������� � ���� ---
    player1 = new HumanPlayer("Player 1");
    player2 = new ComputerPlayer("Computer");
    game = new Game(this);
    game->newGame(player1, player2);

    // --- ����������� �������� ---
    connect(boardWidget, &BoardWidget::cellClicked, this, &GameWindow::handleCellClicked);
    connect(game, &Game::moveResult, this, &GameWindow::showMoveResult);
    connect(game, &Game::boardUpdated, this, &GameWindow::updateBoard);
    connect(game, &Game::gameOver, this, &GameWindow::showGameOver);

    connect(newGameButton, &QPushButton::clicked, [=]() {
        game->newGame(player1, player2);
        infoLabel->setText("New Game!");  // ����� ������
        updateBoard();
    });

    updateBoard(); // ��������� ��������� �����
}

GameWindow::~GameWindow()
{
    delete game;
    delete player1;
    delete player2;
}

// --- ��������� ����� �� ������ ---
void GameWindow::handleCellClicked(int row, int col)
{
    game->processMove(row, col);
}

// --- ���������� ����� ---
void GameWindow::updateBoard()
{
    std::vector<std::vector<int>> state = player1->getBoard().getStateGrid();
    boardWidget->setBoardState(state);
}

// --- ����� ��������� � ���� ---
void GameWindow::showMoveResult(const QString& msg)
{
    infoLabel->setText(msg);
}

// --- ����� ��������� � ���������� ---
void GameWindow::showGameOver(const QString& winner)
{
    infoLabel->setText("Winner: " + winner);
}

