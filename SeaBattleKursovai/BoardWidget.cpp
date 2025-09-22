#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent), cellSize(40), margin(30)
{
    boardState.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    setMinimumSize(GRID_SIZE * cellSize + margin, GRID_SIZE * cellSize + margin);
}

void BoardWidget::setBoardState(const std::vector<std::vector<int>>& state)
{
    boardState = state;
    update();
}

void BoardWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    int availableWidth = width() - margin;
    int availableHeight = height() - margin;
    cellSize = std::min(availableWidth / GRID_SIZE, availableHeight / GRID_SIZE);
    drawCells(painter);
    drawGrid(painter);
    
    drawLabels(painter);
}

void BoardWidget::drawGrid(QPainter& painter)
{
    painter.setPen(Qt::black);
    for (int i = 0; i <= GRID_SIZE; ++i)
    {
        // горизонтальные линии
        painter.drawLine(margin, margin + i * cellSize,
            margin + GRID_SIZE * cellSize, margin + i * cellSize);
        // вертикальные линии
        painter.drawLine(margin + i * cellSize, margin,
            margin + i * cellSize, margin + GRID_SIZE * cellSize);
    }
}

void BoardWidget::drawCells(QPainter& painter)
{
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            QRect cellRect(margin + j * cellSize, margin + i * cellSize, cellSize, cellSize);
            switch (boardState[i][j])
            {
            case 0: painter.fillRect(cellRect, Qt::white); break;
            case 1: painter.fillRect(cellRect, Qt::gray); break;
            case 2: painter.fillRect(cellRect, Qt::blue); break;
            case 3: painter.fillRect(cellRect, Qt::red); break;
            default: painter.fillRect(cellRect, Qt::white); break;
            }
        }
    }
}

void BoardWidget::drawLabels(QPainter& painter)
{
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);

    // цифры слева
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        painter.drawText(5, margin + i * cellSize + cellSize / 2 + 5, QString::number(i + 1));
    }

    // буквы сверху
    for (int j = 0; j < GRID_SIZE; ++j)
    {
        char letter = 'A' + j;
        painter.drawText(margin + j * cellSize + cellSize / 2 - 5, 20, QString(letter));
    }
}

void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    int availableWidth = width() - margin;
    int availableHeight = height() - margin;
    int cellSizeX = availableWidth / GRID_SIZE;
    int cellSizeY = availableHeight / GRID_SIZE;
    cellSize = std::min(cellSizeX, cellSizeY);

    int col = (event->pos().x() - margin) / cellSize;
    int row = (event->pos().y() - margin) / cellSize;

    if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
        return;

    if (event->button() == Qt::LeftButton)
        emit cellClicked(row, col);
    else if (event->button() == Qt::RightButton)
        emit rightClick(row, col);
}




