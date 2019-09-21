#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(560, 500);
    setMouseTracking(true);

    InteractiveButtonBase* btn = new InteractiveButtonBase(this);
    btn->setGeometry(300, 300, 100, 100);
    btn->setBgColor(QColor(128, 0, 0, 100));
    btn->setRadius(5);

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
    text_btn->setGeometry(0, 50, 100, 32);

    icon_btn = new WaterCircleButton(QIcon(":/icons/point_menu"), this);
    icon_btn->setGeometry(100, 50, 32, 32);
    icon_btn->setShowAni(true);

    InteractiveButtonBase* pixmap_btn = new InteractiveButtonBase(QPixmap(":/icons/point_menu"), this);
    pixmap_btn->setGeometry(132, 50, 32, 32);
    pixmap_btn->setDisabled(true);

    InteractiveButtonBase* float_btn = new WaterFloatButton("text", this);
    float_btn->setGeometry(164, 50, 100, 32);
    float_btn->setBgColor(QColor(102,51,204,192), QColor(102,51,204,255));


    PointMenuButton* pm_btn = new PointMenuButton(this);
    pm_btn->setGeometry(200, 200, 100, 100);
}

void MainWindow::enterEvent(QEvent *event)
{
    icon_btn->showForeground2();

    return QMainWindow::enterEvent(event);
}

void MainWindow::leaveEvent(QEvent *event)
{
    icon_btn->hideForeground();

    return QMainWindow::leaveEvent(event);
}
