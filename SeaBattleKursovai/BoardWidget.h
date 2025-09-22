#pragma once
#include <QWidget>
#include <vector>
#include <QMouseEvent>

const int GRID_SIZE = 10; // ��������� ��������� ������� �����

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget* parent = nullptr);

    // ��������� ���������� ������
    void setBoardState(const std::vector<std::vector<int>>& state);

signals:
    void cellClicked(int row, int col);
    void rightClick(int row, int col);

protected:
    void paintEvent(QPaintEvent* event) override; // ��������� override
    void mousePressEvent(QMouseEvent* event) override;

private:
    int cellSize;
    int margin;
    std::vector<std::vector<int>> boardState;

    // ��������� ���������� ��������� �������
    void drawGrid(QPainter& painter);
    void drawCells(QPainter& painter);
    void drawLabels(QPainter& painter);
};
