#ifndef WINSIDEBARBUTTON_H
#define WINSIDEBARBUTTON_H

#include <QObject>
#include <QWidget>
#include "interactivebuttonbase.h"

class WinSidebarButton : public InteractiveButtonBase
{
public:
    WinSidebarButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*event);
    void slotClicked();
};

#endif // WINSIDEBARBUTTON_H
