#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <interactivebuttonbase.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    InteractiveButtonBase* btn;
};

#endif // MAINWINDOW_H
