#include "winmenubutton.h"

WinMenuButton::WinMenuButton(QWidget* parent)
    : InteractiveButtonBase (parent)
{

}

void WinMenuButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();

    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawLine(QPoint(w/3,h/3), QPoint(w*2/3,h/3));
    painter.drawLine(QPoint(w/3,h/2), QPoint(w*2/3,h/2));
    painter.drawLine(QPoint(w/3,h*2/3), QPoint(w*2/3,h*2/3));
}
