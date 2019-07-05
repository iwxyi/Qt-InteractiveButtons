#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(500, 500);

    InteractiveButtonBase* btn = new InteractiveButtonBase(this);
    btn->setGeometry(300, 300, 100, 100);

    WinMinButton *min_btn = new WinMinButton(this);
    min_btn->setGeometry(10, 10, 100, 100);

}
