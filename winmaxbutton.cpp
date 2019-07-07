#include "winmaxbutton.h"

WinMaxButton::WinMaxButton(QWidget *parent)
    : InteractiveButtonBase (parent)
{

}

void WinMaxButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();
    QRect r(w/3+offset_pos.x(), h/3+offset_pos.y(), w/3, h/3);

    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawRect(r);
}
