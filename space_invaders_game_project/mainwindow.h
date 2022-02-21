#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "engine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Функции, возвращающие глобальные переменные (для внешних объектов)
    int SCREEN_WIDTH_PX();
    int SCREEN_HEIGHT_PX();

    // Функции отрисовки и обработки нажатий
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent* event);

public slots:
    void update_game();

private:
    Ui::MainWindow *ui;

    // Глобальные переменные
    int _SCREEN_WIDTH_PX = 800;
    int _SCREEN_HEIGHT_PX = 600;
    int _UPDATE_DELAY_MS = 40;
    Engine engine = Engine(_SCREEN_WIDTH_PX, _SCREEN_HEIGHT_PX);
};
#endif // MAINWINDOW_H
