#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interactivebuttonbase.h"
#include "winminbutton.h"
#include "winmaxbutton.h"
#include "winrestorebutton.h"
#include "winclosebutton.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

};

#endif // MAINWINDOW_H
