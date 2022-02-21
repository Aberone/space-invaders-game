#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Задание размеров и названия игрового окна
    w.setFixedSize(w.SCREEN_WIDTH_PX(), w.SCREEN_HEIGHT_PX());
    w.setWindowTitle("Space Invaders (RyabovAM 2MO)");

    w.show();
    return a.exec();
}
