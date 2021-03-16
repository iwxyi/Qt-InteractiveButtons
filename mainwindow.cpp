#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setFixedSize(560, 500);
    QScreen* desktop = QApplication::screenAt(QCursor::pos());
    QRect rect = desktop->availableGeometry();
    move(rect.left()+(rect.width() - width())/2, (rect.height() - height())/2);

    setMouseTracking(true);

    InteractiveButtonBase* btn0 = new InteractiveButtonBase(this);
    btn0->setGeometry(0, 0, 120, 32);
    btn0->setIcon(QIcon(":/icons/point_menu"));

    InteractiveButtonBase* btn = new InteractiveButtonBase(this);
    btn->setGeometry(110, 100, 100, 100);
    btn->setBgColor(QColor(128, 0, 0, 100));
    btn->setRadius(5);

    AppendButton* append_btn = new AppendButton(this);
    append_btn->setGeometry(304, 10, 32, 32);

    InfoButton* info_btn = new InfoButton(this);
    info_btn->setGeometry(336, 10, 32, 32);

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
    text_btn->setHoverAniDuration(5000);

    icon_btn = new WaterCircleButton(QIcon(":/icons/point_menu"), this);
    icon_btn->setGeometry(100, 50, 32, 32);
    icon_btn->setShowAni(true);
    icon_btn->setFixedForePos(true); // 和上面结合，即从中心开始出现和消失
//    icon_btn->setStyleSheet("qproperty-fore_enabled: false; ");

    InteractiveButtonBase* pixmap_btn = new InteractiveButtonBase(QPixmap(":/icons/point_menu"), this);
    pixmap_btn->setGeometry(132, 50, 32, 32);
    pixmap_btn->setDisabled(true);

    InteractiveButtonBase* float_btn = new WaterFloatButton("text", this);
    float_btn->setGeometry(164, 50, 100, 32);
    float_btn->setBgColor(QColor(102,51,204,192), QColor(102,51,204,255));
    float_btn->setIconColor(QColor(102,51,204,192));
    float_btn->setStyleSheet("qproperty-text: abcdefg; qproperty-water_animation: true; ");

    InteractiveButtonBase* par_btn = new InteractiveButtonBase("parent", this);
    par_btn->setGeometry(270, 50, 100, 32);
    par_btn->setParentEnabled(true);
    par_btn->setForeEnabled(false);
    par_btn->setStyleSheet("qproperty-text: asdasd; color: red; qproperty-press_color: green;");

    QPushButton* push_btn = new QPushButton("parent", this);
    push_btn->setGeometry(370, 50, 100, 32);

    PointMenuButton* pm_btn = new PointMenuButton(this);
    pm_btn->setGeometry(0, 100, 100, 100);
    pm_btn->setStyleSheet("qproperty-background_color: red; qproperty-radius: 20; qproperty-border_color: green; qproperty-border_width:3;"
                          "qproperty-icon_color: blue; ");

    ThreeDimenButton* tdb = new ThreeDimenButton(this);
    tdb->setGeometry(220, 100, 200, 100);
    tdb->setBgColor(Qt::gray);
    connect(tdb, &ThreeDimenButton::clicked, [=]{ qDebug() << "3D按钮clicked"; });
    connect(tdb, &ThreeDimenButton::signalMouseEnter, [=]{ qDebug() << "mouseEnter"; });
    connect(tdb, &ThreeDimenButton::signalMouseLeave, [=]{ qDebug() << "mouseLeave"; });
    connect(tdb, &ThreeDimenButton::signalMouseEnterLater, [=]{ qDebug() << "mouseEnterLater"; });
    connect(tdb, &ThreeDimenButton::signalMouseLeaveLater, [=]{ qDebug() << "mouseLeaveLater"; });
    connect(tdb, &ThreeDimenButton::signalMousePress, [=](QMouseEvent*){ qDebug() << "mousePress"; });
    connect(tdb, &ThreeDimenButton::signalMouseRelease, [=](QMouseEvent*){ qDebug() << "mouseRelease"; });
    connect(tdb, &ThreeDimenButton::signalMousePressLater, [=](QMouseEvent*){ qDebug() << "mousePressLater"; });
    connect(tdb, &ThreeDimenButton::signalMouseReleaseLater, [=](QMouseEvent*){ qDebug() << "mouseReleaseLater"; });

    WaterZoomButton* zoom_btn1 = new WaterZoomButton("tttttttttt", this);
    zoom_btn1->setGeometry(300, 200, 200, 50);
    zoom_btn1->setBgColor(QColor(240,128,128));
    zoom_btn1->setBgColor(Qt::transparent, QColor(0x88, 0x88, 0x88, 0x64));
    zoom_btn1->setRadius(10, 5);
    zoom_btn1->setChokingProp(0.18);

    InteractiveButtonBase* double_btn = new InteractiveButtonBase(this);
    double_btn->setGeometry(510, 200, 50, 50);
    double_btn->setDoubleClicked(true);
    double_btn->setBgColor(QColor(102,51,204,192));
    connect(double_btn, &InteractiveButtonBase::clicked, [=]{
        qDebug() << "单击";
    });
    connect(double_btn, &InteractiveButtonBase::doubleClicked, [=]{
        qDebug() << "双击";
    });

    InteractiveButtonBase* icon_text_btn = new InteractiveButtonBase(QIcon(":/icons/point_menu"), "菜单", this);
    icon_text_btn->setGeometry(100, 210, 100, 40);
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
