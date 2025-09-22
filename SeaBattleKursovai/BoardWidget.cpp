#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <QDebug>

BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent), cellSize(40), margin(30)
{
    boardState.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    setMinimumSize(GRID_SIZE * cellSize + 2 * margin, GRID_SIZE * cellSize + 2 * margin);
}

void BoardWidget::setBoardState(const std::vector<std::vector<int>>& state)
{
    if (state.size() != GRID_SIZE || (state.size() > 0 && state[0].size() != GRID_SIZE)) {
        qDebug() << "Invalid board state size:" << state.size()
            << "x" << (state.empty() ? 0 : state[0].size());
        return;
    }

    boardState = state;
    update();
}

void BoardWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int availableWidth = width() - 2 * margin;
    int availableHeight = height() - 2 * margin;
    cellSize = std::min(availableWidth / GRID_SIZE, availableHeight / GRID_SIZE);

    drawCells(painter);
    drawGrid(painter);
    drawLabels(painter);
}

void BoardWidget::drawGrid(QPainter& painter)
{
    painter.setPen(QPen(Qt::black, 2));

    for (int i = 0; i <= GRID_SIZE; i++) {
        int x = margin + i * cellSize;
        painter.drawLine(x, margin, x, margin + GRID_SIZE * cellSize);
    }

    for (int i = 0; i <= GRID_SIZE; i++) {
        int y = margin + i * cellSize;
        painter.drawLine(margin, y, margin + GRID_SIZE * cellSize, y);
    }
}

void BoardWidget::drawCells(QPainter& painter)
{
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            int x = margin + col * cellSize;
            int y = margin + row * cellSize;
            QRect cellRect(x, y, cellSize, cellSize);

            switch (boardState[row][col]) {
            case 0: painter.fillRect(cellRect, Qt::white); break;
            case 1: painter.fillRect(cellRect, Qt::gray); break;
            case 2: painter.fillRect(cellRect, Qt::blue); break;
            case 3: painter.fillRect(cellRect, Qt::red); break;
            default: painter.fillRect(cellRect, Qt::white);
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

    for (int i = 0; i < GRID_SIZE; i++) {
        painter.drawText(5, margin + i * cellSize + cellSize / 2 + 5, QString::number(i + 1));
    }

    for (int j = 0; j < GRID_SIZE; j++) {
        char letter = 'A' + j;
        painter.drawText(margin + j * cellSize + cellSize / 2 - 5, 20, QString(letter));
    }
}

void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    int availableWidth = width() - 2 * margin;
    int availableHeight = height() - 2 * margin;

    if (availableWidth <= 0 || availableHeight <= 0) return;

    cellSize = std::min(availableWidth / GRID_SIZE, availableHeight / GRID_SIZE);
    if (cellSize <= 0) return;

    int col = (event->pos().x() - margin) / cellSize;
    int row = (event->pos().y() - margin) / cellSize;

    qDebug() << objectName() << "click - raw:" << event->pos().x() << event->pos().y()
        << "computed:" << row << col << "margin:" << margin << "cellSize:" << cellSize;

    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
        if (event->button() == Qt::LeftButton) {
            emit cellClicked(row, col);
        }
        else if (event->button() == Qt::RightButton) {
            emit rightClick(row, col);
        }
    }
    else {
        qDebug() << "Click outside board";
    }
}




