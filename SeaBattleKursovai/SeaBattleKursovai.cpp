#include "SeaBattleKursovai.h"
#include <QPushButton>  // подключаем класс кнопки для работы с её свойствами

SeaBattleKursovai::SeaBattleKursovai(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this); // загружаем интерфейс из .ui

    // Связываем сигнал нажатия кнопки со слотом
    connect(ui.pushButton, &QPushButton::clicked, this, &SeaBattleKursovai::onButtonClicked);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &SeaBattleKursovai::onButtonClicked);
}

SeaBattleKursovai::~SeaBattleKursovai()
{
}

void SeaBattleKursovai::onButtonClicked()
{
    // Узнаём, кто нажал кнопку
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        button->setStyleSheet("background-color: red; color: white;");
    }
}

