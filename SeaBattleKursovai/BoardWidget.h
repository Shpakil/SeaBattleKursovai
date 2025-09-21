#pragma once

#include <QWidget>
#include <vector>

class BoardWidget : public QWidget
{
    Q_OBJECT

private:
    static const int GRID_SIZE = 10;   // размер доски 10x10
    int cellSize;                       // размер одной клетки
    std::vector<std::vector<int>> boardState; // 0 = пусто, 1 = корабль, 2 = промах, 3 = попадание

public:
    explicit BoardWidget(QWidget* parent = nullptr);

    void setBoardState(const std::vector<std::vector<int>>& state); // обновление состояния доски

signals:
    void cellClicked(int row, int col); // сигнал при клике на клетку

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

