#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SeaBattleKursovai.h"

class SeaBattleKursovai : public QMainWindow
{
    Q_OBJECT

public:
    SeaBattleKursovai(QWidget *parent = nullptr);
    ~SeaBattleKursovai();

private slots:  // <-- �������� ������ ��� ������
    void onButtonClicked(); // <-- ��� ����� ��� ������� ������

private:
    Ui::SeaBattleKursovaiClass ui;
};

