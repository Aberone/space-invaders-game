#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QImage>
#include <QString>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Место для подключения слотов
    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update_game()));
    timer->start(_UPDATE_DELAY_MS);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::SCREEN_WIDTH_PX()
{
    return this->_SCREEN_WIDTH_PX;
}

int MainWindow::SCREEN_HEIGHT_PX()
{
    return this->_SCREEN_HEIGHT_PX;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    engine.draw_game(&painter);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        engine.try_update_keyboard_state("space", true);
    }
    else if (event->key() == Qt::Key_Left) {
        engine.try_update_keyboard_state("left", true);
    }
    else if (event->key() == Qt::Key_Right) {
        engine.try_update_keyboard_state("right", true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        engine.try_update_keyboard_state("space", false);
    }
    else if (event->key() == Qt::Key_Left) {
        engine.try_update_keyboard_state("left", false);
    }
    else if (event->key() == Qt::Key_Right) {
        engine.try_update_keyboard_state("right", false);
    }
}

void MainWindow::update_game()
{
    engine.init_turn();
    repaint();

    if (engine.is_game_over()) {
        QMessageBox msgbox;
        msgbox.setText("Game Over!");
        msgbox.exec();
        qApp->quit();
    }
    else if (engine.is_victory()) {
        QMessageBox msgbox;
        msgbox.setText("VICTORY!!!");
        msgbox.exec();
        qApp->quit();
    }
}
