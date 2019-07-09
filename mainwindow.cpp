#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(560, 500);

    InteractiveButtonBase* btn = new InteractiveButtonBase(this);
    btn->setGeometry(300, 300, 100, 100);

    WinMenuButton* menu_btn = new WinMenuButton(this);
    menu_btn->setGeometry(368, 10, 32, 32);

    WinMinButton *min_btn = new WinMinButton(this);
    min_btn->setGeometry(400, 10, 32, 32);

    WinMaxButton *max_btn = new WinMaxButton(this);
    max_btn->setGeometry(432, 10, 32, 32);

    WinRestoreButton* res_btn = new WinRestoreButton(this);
    res_btn->setGeometry(464, 10, 32, 32);

    WinCloseButton* close_btn = new WinCloseButton(this);
    close_btn->setGeometry(496, 10, 32, 32);
    close_btn->setBgColor(Qt::black, Qt::red);

    WaterCircleButton* cir_btn = new WaterCircleButton(this);
    cir_btn->setGeometry(528, 10, 32, 32);

    InteractiveButtonBase* text_btn = new InteractiveButtonBase("text", this);
    text_btn->setGeometry(100, 50, 32, 32);
}
