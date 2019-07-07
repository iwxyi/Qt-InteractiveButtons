#ifndef WINMENUBUTTON_H
#define WINMENUBUTTON_H

#include <QObject>
#include <QWidget>
#include "interactivebuttonbase.h"

class WinMenuButton : public InteractiveButtonBase
{
public:
    WinMenuButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*event);
};

#endif // WINMENUBUTTON_H
