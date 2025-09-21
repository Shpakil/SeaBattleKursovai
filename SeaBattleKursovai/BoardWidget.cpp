#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>

BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent), cellSize(40)
{
    // ������������� ������ �����
    boardState.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    setFixedSize(GRID_SIZE * cellSize, GRID_SIZE * cellSize);
}

void BoardWidget::setBoardState(const std::vector<std::vector<int>>& state)
{
    boardState = state;
    update(); // ������������ �����
}

void BoardWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            QRect cellRect(j * cellSize, i * cellSize, cellSize, cellSize);

            // ���� ������ � ����������� �� ���������
            switch (boardState[i][j])
            {
            case 0: painter.fillRect(cellRect, Qt::white); break;     // �����
            case 1: painter.fillRect(cellRect, Qt::gray); break;      // ������� (����� ������ ��� ����������)
            case 2: painter.fillRect(cellRect, Qt::blue); break;      // ������
            case 3: painter.fillRect(cellRect, Qt::red); break;       // ���������
            }

            painter.drawRect(cellRect); // ������� ������
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    int col = event->pos().x() / cellSize;
    int row = event->pos().y() / cellSize;

    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
        emit cellClicked(row, col);
}

