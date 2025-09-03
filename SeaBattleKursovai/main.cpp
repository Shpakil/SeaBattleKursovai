#include "SeaBattleKursovai.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SeaBattleKursovai window;
    window.show();
    return app.exec();
}
