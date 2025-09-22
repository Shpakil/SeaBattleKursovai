#pragma once
#include <QWidget>
#include <vector>
#include <QMouseEvent>

const int GRID_SIZE = 10; // Добавляем константу размера сетки

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget* parent = nullptr);

    // Добавляем объявление метода
    void setBoardState(const std::vector<std::vector<int>>& state);

signals:
    void cellClicked(int row, int col);
    void rightClick(int row, int col);

protected:
    void paintEvent(QPaintEvent* event) override; // Добавляем override
    void mousePressEvent(QMouseEvent* event) override;

private:
    int cellSize;
    int margin;
    std::vector<std::vector<int>> boardState;

    // Добавляем объявления приватных методов
    void drawGrid(QPainter& painter);
    void drawCells(QPainter& painter);
    void drawLabels(QPainter& painter);
};
