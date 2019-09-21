#ifndef WINCLOSEBUTTON_H
#define WINCLOSEBUTTON_H

#include <QWidget>
#include <QObject>
#include "interactivebuttonbase.h"

class WinCloseButton : public InteractiveButtonBase
{
public:
    WinCloseButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*event);
};

#endif // WINCLOSEBUTTON_H
