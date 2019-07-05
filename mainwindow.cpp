#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(500, 500);

    btn = new InteractiveButtonBase(this);
    btn->setFixedSize(100, 100);
    btn->move(this->geometry().center());
    btn->show();
}
