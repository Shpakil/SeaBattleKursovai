#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SeaBattleKursovai.h"

class SeaBattleKursovai : public QMainWindow
{
    Q_OBJECT

public:
    SeaBattleKursovai(QWidget *parent = nullptr);
    ~SeaBattleKursovai();

private slots:  // <-- Добавили секцию для слотов
    void onButtonClicked(); // <-- Наш метод для нажатия кнопки

private:
    Ui::SeaBattleKursovaiClass ui;
};

