#pragma once

#include <QWidget>
#include <vector>

class BoardWidget : public QWidget
{
    Q_OBJECT

private:
    static const int GRID_SIZE = 10;   // ������ ����� 10x10
    int cellSize;                       // ������ ����� ������
    std::vector<std::vector<int>> boardState; // 0 = �����, 1 = �������, 2 = ������, 3 = ���������

public:
    explicit BoardWidget(QWidget* parent = nullptr);

    void setBoardState(const std::vector<std::vector<int>>& state); // ���������� ��������� �����

signals:
    void cellClicked(int row, int col); // ������ ��� ����� �� ������

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

