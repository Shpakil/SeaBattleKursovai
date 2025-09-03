#include "SeaBattleKursovai.h"
#include <QPushButton>  // ���������� ����� ������ ��� ������ � � ����������

SeaBattleKursovai::SeaBattleKursovai(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this); // ��������� ��������� �� .ui

    // ��������� ������ ������� ������ �� ������
    connect(ui.pushButton, &QPushButton::clicked, this, &SeaBattleKursovai::onButtonClicked);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &SeaBattleKursovai::onButtonClicked);
}

SeaBattleKursovai::~SeaBattleKursovai()
{
}

void SeaBattleKursovai::onButtonClicked()
{
    // �����, ��� ����� ������
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        button->setStyleSheet("background-color: red; color: white;");
    }
}

