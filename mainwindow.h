#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interactivebuttonbase.h"
#include "winminbutton.h"
#include "winmaxbutton.h"
#include "winrestorebutton.h"
#include "winclosebutton.h"
#include "winmenubutton.h"
#include "watercirclebutton.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private:
    InteractiveButtonBase* icon_btn;

};

#endif // MAINWINDOW_H
